//
// Created by loyusum on 9/6/2020.
//

#include "UserClient.hpp"

UserClient::UserClient(std::string server_ip, int port) {

    m_tcp_client = ToolBox::make_unique<TCPClient>();

    const auto result = m_tcp_client->connect(std::move(server_ip), port);

    if (result != -1) {

        m_has_connect_server = true;

    }

}

bool UserClient::HasConnect() {

    return m_has_connect_server;

}

bool UserClient::login(AccountData &accountData) {

    std::string account_data_str =
            Constant::frag_server_login + accountData.m_user_name + Constant::data_spilt + accountData.m_password;

    Socket::Message mes;

    mes.mes.assign(account_data_str.begin(), account_data_str.begin() + account_data_str.size());

    // tell server we need to login
    m_tcp_client->send(mes);

    // server tell us whether we successfully login or not
    m_tcp_client->receive(mes);

    std::string recv_message(mes.mes.begin(), mes.mes.end());
    std::string posi_str(Constant::positive_message,
                         Constant::positive_message + sizeof(Constant::positive_message));

    // if message is positive
    if (posi_str == recv_message) {

        accountData.m_is_login = true;

        return true;

    }

    accountData.m_is_login = false;
    return false;
}

void UserClient::logout() {

#warning logout not finished

}

void UserClient::SendAccountDataToServer_(AccountData &accountData) {

    std::string attention_data = DataCollector::GenMessageData(accountData);

    std::string send_data_str = Constant::frag_upload_user_data + attention_data;

    Socket::Message mes;

    mes.mes.assign(send_data_str.begin(), send_data_str.end());

    // send attention message to server
    m_tcp_client->send(mes);
}

UserClient::~UserClient() {

    m_listen_stop = true;

    if (HasConnect()) {
        // tell server we're leaving
        Socket::Message mes;
        mes.mes.resize(1, Constant::frag_user_leave);
        m_tcp_client->send(mes);
    }

    m_has_connect_server = false;

    if (m_listen_thread) {

        // thread has not ended
        if (m_listen_thread->joinable()) {

            m_listen_thread->join();
        }
    }
}

void UserClient::Start_() {

    Socket::Message mes;

    static constexpr int MAX_EVENT = 8;
    epoll_event events[MAX_EVENT];

    /*---Add socket to epoll---*/
    int epfd = epoll_create(1);
    struct epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = m_tcp_client->GetFD();
    epoll_ctl(epfd, EPOLL_CTL_ADD, event.data.fd, &event);

    while (!m_listen_stop) {
        ListenLock();

        int number_ready = epoll_wait(epfd, events, MAX_EVENT, 1000/*timeout*/);

        // have recv one or more message
        for (int i = 0; i < number_ready; i++) {
            if (events[i].events & EPOLLIN) {

                int size = m_tcp_client->receive(mes);

                // disconnected
                if (size == 0) {

                    ToolBox::log() << "Socket closed, stop listening." << std::endl;
                    m_has_connect_server = false;
                    return;
                }

                //ToolBox::log() << "We get a message from server with size [" << mes.mes.size() << "]"
                //               << std::endl;

                m_server_message.emplace_back(std::move(mes));
            }
        }

        ListenUnLock();

        // for other thread to take over the lock if necessary
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void UserClient::Start() {

    if (m_listen_thread) {

        // thread has not ended
        if (m_listen_thread->joinable()) {

            return;
        }
    }

    m_listen_thread = ToolBox::make_unique<std::thread>(&UserClient::Start_, this);
}

void UserClient::Update() {

    MessageHandle_();
}

void UserClient::MessageHandle_() {

    if (m_server_message.empty()) {

        return;
    }

    // get the first message from the front
    auto server_message = std::move(m_server_message.front());
    m_server_message.pop_front();

    switch (server_message.mes[0]) {

        case Constant::frag_user_leave: {

            ToolBox::log() << "Server leaved." << std::endl;
            exit(0);

        }

            break;

        case Constant::frag_image: {


            server_message.mes.erase(server_message.mes.begin());// delete frag

            sf::Image recv_image;
            MessagePackage::ReadImage(server_message, recv_image);

            if (m_meeting_core)
                m_meeting_core->m_main_window->SetImage(recv_image);

        }

        case Constant::frag_image_jpg_file: {

            server_message.mes.erase(server_message.mes.begin());// delete frag

            MessagePackage::ReadFile(server_message, "resource/tem_recv.jpg");
            sf::Image recv_image;

            recv_image.loadFromFile("resource/tem_recv.jpg");
            if (m_meeting_core)
                m_meeting_core->m_main_window->SetImage(recv_image);
        }

            break;

        default:
            ToolBox::err() << "Unknown message format receive from server. " << std::endl;

            break;

    }
}

void UserClient::SetMeetingCore(EMCMeeting *emcMeeting) {

    m_meeting_core = emcMeeting;

}

void UserClient::ListenLock() {
    m_listen_thread_mutex.lock();

}

void UserClient::ListenUnLock() {
    m_listen_thread_mutex.unlock();
}
