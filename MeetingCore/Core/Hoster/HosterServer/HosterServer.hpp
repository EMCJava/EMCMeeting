//
// Created by loyusum on 7/6/2020.
//

#ifndef EMCMEETING_HOSTERSERVER_HPP
#define EMCMEETING_HOSTERSERVER_HPP

#include <thread>
#include <memory>
#include <vector>
#include <deque>
#include <chrono> // for system clock

#include "../../../../ToolBox/ToolBox.hpp"
#include "../../../../ToolBox/Constant.hpp"

#include "../../NetWork/Socket.hpp"
#include "../../NetWork/TCPServer.hpp"

// hoster can collect the active data from user
#include "../UserDataBase/UserDataBase.hpp"
#include "../DataCollector/DataCollector.hpp"

class HosterServer {

private:

    std::unique_ptr<UserDataBase> m_user_data_base;

    // the Tcp socket class
    std::unique_ptr<TCPServer> m_tcp_server;

    // send & recv & accept client thread
    bool m_listen_stop = false;
    std::unique_ptr<std::thread> m_listen_thread;

    // client content
    struct Client {

        int sockfd;

        std::string name, password;

        bool has_login = false;
        typeof(std::chrono::system_clock::now()) connect_time_point;

        explicit Client(int fd, bool login = false) {
            sockfd = fd;
            has_login = login;
        }

    };

    // store all the client that has accept
    std::vector<Client> m_clients;

    // store all the data from all client e.g. concentration
    std::unique_ptr<DataCollector> m_data_collector;

    struct ClientMessage {

        Socket::Message mes;
        int client_index = -1;

        ClientMessage(Socket::Message &&message, int index) {
            mes = std::move(message);
            client_index = index;
        }
    };

    // store all the message from client
    std::deque<ClientMessage> m_client_message;

    void MessageHandle_();

    unsigned int m_max_client = 10;

    void Start_(int max_client);

    void ResetClient_(Client& client);

public:

    explicit HosterServer(unsigned int max_client);

    void Start();

    void Update();
};


#endif //EMCMEETING_HOSTERSERVER_HPP
