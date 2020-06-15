//
// Created by loyusum on 7/6/2020.
//

#include "HosterServer.hpp"


HosterServer::HosterServer(unsigned int max_client) : m_max_client(max_client) {

#ifdef USE_STATIC_IP

#warning using ip 127.0.0.1

    m_tcp_server = ToolBox::make_unique<TCPServer>(Constant::SOCKET_IP, Constant::SOCKET_PORT);

#endif

    m_data_collector = ToolBox::make_unique<DataCollector>();
    m_clients.resize(m_max_client, Client{Socket::EmptySock(), false});

    m_user_data_base = ToolBox::make_unique<UserDataBase>(std::string(Constant::path_user_account_data));
}


void HosterServer::Start() {

    if (m_listen_thread) {

        // thread has not ended
        if (m_listen_thread->joinable()) {

            return;
        }
    }

    m_tcp_server->listen(m_max_client);
    m_listen_thread = ToolBox::make_unique<std::thread>(&HosterServer::Start_, this, m_max_client);

}

void HosterServer::Start_(int max_client) {

    static constexpr auto microseconds_to_second_ratio = (float) std::chrono::microseconds::period::num /
                                                         std::chrono::microseconds::period::den;

    // + 1 for server itself
    const auto max_epoll_size = max_client + 1;

    const int sock_fd = m_tcp_server->GetFD();
    int max_sd, sd, new_socket;
    Socket::Message mes;

    struct sockaddr_in address{};
    socklen_t addrlen = 0;

    struct epoll_event ev{};
    std::vector<epoll_event> events;

    // + 1 for server itself
    events.resize(max_epoll_size);

    int kdpfd = epoll_create(max_epoll_size);
    ev.events = EPOLLIN;
    ev.data.fd = sock_fd;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, sock_fd, &ev) < 0) {
        ToolBox::err() << "epoll set insertion error: fd = " << sock_fd << std::endl;
        ToolBox::err() << "Stop listening client message" << sock_fd << std::endl;
        return;
    }

    int curfds = 1;

    while (!m_listen_stop) {

        auto nfds = epoll_wait(kdpfd, events.data(), curfds, 1000/*timeout*/);
        //ToolBox::log() << "Start epoll_wait for one second, got : " << nfds << std::endl;

        if (nfds == -1) {
            ToolBox::err() << "epoll_wait error" << std::endl;
            continue;
        }

        // loop all event
        for (int n = 0; n < nfds; ++n) {
            // is server has input stream
            if (events[n].data.fd == sock_fd) {

                if ((new_socket = m_tcp_server->accept()) < 0) {
                    ToolBox::err() << "Can't accept client, check your connection" << std::endl;

                    continue;
                }

                ToolBox::log() << "Found a new client" << std::endl;

                bool has_added_client = false;

                //add new socket to array of sockets
                for (auto &client : m_clients) {
                    //if position is empty
                    if (client.sockfd == Socket::EmptySock()) {
                        client.sockfd = new_socket;
                        client.has_login = false;
                        client.connect_time_point = std::chrono::system_clock::now();

                        ToolBox::log() << "New client adding to list of sockets as sockfd " << client.sockfd
                                       << std::endl;
                        has_added_client = true;
                        break;
                    }
                }

                // dont have space for new client
                if (!has_added_client) {

                    mes.mes.assign(Constant::server_disconnect_full,
                                   Constant::server_disconnect_full + sizeof(Constant::server_disconnect_full));

                    // stop client from keep connecting
                    m_tcp_server->send(new_socket, mes);

                    close(new_socket);

                    continue;
                }

                ev.events = EPOLLIN;
                ev.data.fd = new_socket;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_socket, &ev) < 0) {
                    ToolBox::err() << "add socket " << new_socket << " to epoll failed: " << strerror(errno)
                                   << std::endl;

                    return;
                }
                curfds++;
                continue;
            }

            //else its some IO operation on some other socket
            sd = events[n].data.fd;

            //Check if it was for closing , and also read the
            //incoming message

            int result;
            if ((result = m_tcp_server->receive(sd, mes)) == 0 && result == -1) {
                //Somebody disconnected , get his details and print
                getpeername(sd, (struct sockaddr *) &address,
                            (socklen_t *) &addrlen);

                auto remove_target_client = std::find_if(m_clients.begin(), m_clients.end(),
                                                         [sd](HosterServer::Client &client) -> bool {
                                                             return client.sockfd == sd;
                                                         });

                ToolBox::log() << "Host disconnected , ip " << inet_ntoa(address.sin_addr) <<
                               " , port " << ntohs(address.sin_port) <<
                               " , user " << remove_target_client->name << std::endl;

                //Close the socket and mark as Socket::EmptySock() in list for reuse

                ResetClient_(*remove_target_client);

                epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
                curfds--;
            } else { // we get a message !
                ToolBox::log() << "We get a message from " << sd << " with size [" << mes.mes.size() << "]"
                               << std::endl;

                m_client_message.emplace_back(std::move(mes), sd);
            }

        }

        const auto current_time = std::chrono::system_clock::now();

        // if some client didn't login after a long time
        for (auto &client : m_clients) {

            if (client.has_login || client.sockfd == Socket::EmptySock()) {
                continue;
            }

            //haven login

            const auto passed_time = current_time - client.connect_time_point;

            // get the idle time in second (float from)
            const auto passed_time_float = (float) std::chrono::duration_cast<std::chrono::microseconds>(
                    passed_time).count() * microseconds_to_second_ratio;

            // idle for too long
            if (passed_time_float > Constant::IDLE_KICK_TIME_SECOND) {

                getpeername(sd, (struct sockaddr *) &address,
                            (socklen_t *) &addrlen);

                ToolBox::log() << "Host idle , ip " << inet_ntoa(address.sin_addr) <<
                               " , port " << ntohs(address.sin_port) << " has been kicked." << std::endl;

                mes.mes.assign(Constant::server_disconnect_idle,
                               Constant::server_disconnect_idle + sizeof(Constant::server_disconnect_idle));

                // stop client from keep connecting
                m_tcp_server->send(new_socket, mes);

                // close socket and reset

                epoll_ctl(kdpfd, EPOLL_CTL_DEL, client.sockfd, &ev);
                curfds--;
                ResetClient_(client);

            }
        }

    }

}


void HosterServer::Update() {

    // recv any data from client


    MessageHandle_();
}

void HosterServer::MessageHandle_() {

    if (m_client_message.empty()) {

        return;
    }

    // get the first message from the front
    auto client_message = std::move(m_client_message.front());
    m_client_message.pop_front();

    int client_fd = client_message.client_fd;
    auto client_it = std::find_if(m_clients.begin(), m_clients.end(),
                                             [client_fd](HosterServer::Client &client) -> bool {
                                                 return client.sockfd == client_fd;
                                             });

    switch (client_message.mes.mes[0]) {

        case Constant::frag_server_login: {

            // skip the first character
            std::string pure_data =
                    std::string(client_message.mes.mes.data(),
                                client_message.mes.mes.data() +
                                client_message.mes.mes.size()) // since it might not ends with '\0'
                            .substr(1);


            // get user name and password data
            // format username + spilt_pos + password
            const auto spilt_pos = pure_data.find(Constant::data_spilt);

            client_it->name = pure_data.substr(0, spilt_pos);
            client_it->password = pure_data.substr(spilt_pos + 1);

            if (client_it->password.back() == '\0') {
                client_it->password.pop_back();
            }

            //check if is user name and password is available
            bool account_exist = m_user_data_base->UserAllowLogin(client_it->name, client_it->password);

            const auto &message = account_exist ? Constant::positive_message
                                                : Constant::negative_message;

            // give feedback to client
            client_message.mes.mes.assign(message, message + sizeof(message));
            m_tcp_server->send(client_it->sockfd, client_message.mes);

            // give user feedback
            ToolBox::log() << "Account " << client_it->name << " " << message << std::endl;

            client_it->has_login = account_exist;

#warning testing image
            if (account_exist) {

                Socket::Message image_message;
                sf::Image im;
                im.loadFromFile("resource/testing1.png");

                sf::Image send_im;
                send_im.create(800, 600);
                MessagePackage::resizeImage(im, send_im);

                send_im.saveToFile("resource/tem_send.jpg");

                MessagePackage::GenMessage(image_message, "resource/tem_send.jpg");

                image_message.mes.insert(image_message.mes.begin(), Constant::frag_image_jpg_file);

                m_tcp_server->send(client_it->sockfd, image_message);
            }

        }

            break;

        case Constant::frag_upload_user_data: {

            // skip the first character
            std::string pure_data =
                    std::string(client_message.mes.mes.data(),
                                client_message.mes.mes.data() +
                                client_message.mes.mes.size()) // since it might not ends with '\0'
                            .substr(1);


            m_data_collector->m_clients_data[client_it->name] = DataCollector::DecodeMessageData(pure_data);
            ToolBox::log() << "User " << client_it->name << " sending account data , updating client account data."
                           << std::endl;

        }

            break;

        case Constant::frag_user_leave: {

            ToolBox::log() << "User " << client_it->name << " leave , resetting client data." << std::endl;
            ResetClient_(*client_it);

        }

            break;

        default:
            ToolBox::err() << "Unknown message format receive from client. " << std::endl;

            break;

    }

}

void HosterServer::ResetClient_(HosterServer::Client &client) {

    if (client.sockfd != Socket::EmptySock())
        close(client.sockfd);

    client.sockfd = Socket::EmptySock();
    client.has_login = false;

}

HosterServer::~HosterServer() {
    m_listen_stop = true;

    Socket::Message mes;
    mes.mes.resize(1, Constant::frag_user_leave);

    for (auto &client : m_clients) {

        if (client.sockfd == Socket::EmptySock()) {
            continue;
        }

        // tell all client, server is leaving
        m_tcp_server->send(client.sockfd, mes);
    }

    if (m_listen_thread) {

        // thread has not ended
        if (m_listen_thread->joinable()) {

            m_listen_thread->join();
        }
    }
}
