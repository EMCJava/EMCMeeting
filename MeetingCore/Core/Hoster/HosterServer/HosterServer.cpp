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

        return;

    }

    m_tcp_server->listen(m_max_client);
    m_listen_thread = ToolBox::make_unique<std::thread>(&HosterServer::Start_, this, m_max_client);

}

void HosterServer::Start_(int max_client) {

    static constexpr auto microseconds_to_second_ratio = (float) std::chrono::microseconds::period::num /
                                                         std::chrono::microseconds::period::den;

    fd_set readfds;

    int sock_fd = m_tcp_server->GetFD(), max_sd, sd, new_socket;
    Socket::Message mes;

    struct sockaddr_in address{};
    socklen_t addrlen = 0;

    while (!m_listen_stop) {

        //clear the socket set
        FD_ZERO(&readfds);

        //add server socket to set
        FD_SET(sock_fd, &readfds);
        max_sd = sock_fd;

        //add client sockets to set
        for (auto client : m_clients) {
            //socket descriptor
            sd = client.sockfd;

            //if valid socket descriptor then add to read list
            if (sd != Socket::EmptySock())
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        // time out limit since we need to check m_listen_stop
        struct timeval tv{1, 0};

        auto activity = select(max_sd + 1, &readfds, nullptr, nullptr, &tv);
        //ToolBox::log() << "Start select for one second, got : " << activity << std::endl;

        if ((activity < 0) && (errno != EINTR)) {
            ToolBox::err() << "select error" << std::endl;
        }

        if (FD_ISSET(sock_fd, &readfds)) {
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
            }

        }

        //else its some IO operation on some other socket
        for (int i = 0; i < m_clients.size(); ++i) {
            sd = m_clients[i].sockfd;

            if (FD_ISSET(sd, &readfds)) {
                //Check if it was for closing , and also read the
                //incoming message
                if (m_tcp_server->receive(sd, mes) == 0) {
                    //Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr *) &address,
                                (socklen_t *) &addrlen);

                    ToolBox::log() << "Host disconnected , ip " << inet_ntoa(address.sin_addr) <<
                                   " , port " << ntohs(address.sin_port) <<
                                   " , user " << m_clients[i].name << std::endl;

                    //Close the socket and mark as Socket::EmptySock() in list for reuse
                    ResetClient_(m_clients[i]);
                } else {
                    ToolBox::log() << "We get a message from " << sd << " as [" << mes.mes.data() << "]"
                                   << std::endl;

                    m_client_message.emplace_back(std::move(mes), i);
                }
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

    auto &client = m_clients[client_message.client_index];

    // skip the first character
    std::string pure_data =
            std::string(client_message.mes.mes.data(),
                        client_message.mes.mes.data() +
                        client_message.mes.mes.size()) // since it might not ends with '\0'
                    .substr(1);

    switch (client_message.mes.mes[0]) {

        case Constant::frag_server_login: {

            // get user name and password data
            // format username + spilt_pos + password
            const auto spilt_pos = pure_data.find(Constant::data_spilt);

            client.name = pure_data.substr(0, spilt_pos);
            client.password = pure_data.substr(spilt_pos + 1);

            if (client.password.back() == '\0') {
                client.password.pop_back();
            }

            //check if is user name and password is available
            bool account_exist = m_user_data_base->UserAllowLogin(client.name, client.password);

            const auto &message = account_exist ? Constant::positive_message
                                                : Constant::negative_message;

            // give feedback to client
            client_message.mes.mes.assign(message, message + sizeof(message));
            m_tcp_server->send(client.sockfd, client_message.mes);

            // give user feedback
            ToolBox::log() << "Account " << client.name << " " << message << std::endl;

            client.has_login = account_exist;

        }

            break;

        case Constant::frag_upload_user_data: {

            m_data_collector->m_clients_data[client.name] = DataCollector::DecodeMessageData(pure_data);
            ToolBox::log() << "User " << client.name << " sending account data , updating client account data."
                           << std::endl;

        }

            break;

        case Constant::frag_user_leave: {

            ToolBox::log() << "User " << client.name << " leave , resetting client data." << std::endl;
            ResetClient_(client);

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
