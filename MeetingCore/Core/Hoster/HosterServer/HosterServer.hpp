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
#include <csignal>

#include "../../../../ToolBox/ToolBox.hpp"
#include "../../../../ToolBox/Constant.hpp"

#include "../../NetWork/Socket.hpp"
#include "../../NetWork/TCPServer.hpp"

#ifdef __linux__

#include <sys/epoll.h>

#elif defined(__WIN32__) || defined(_WIN32)

#include <winsock2.h>
#include <windows.h>

#else

#error sys/epoll.h is for linux

#endif

#include "../../ScreenShot/ScreenShot.hpp"

// hoster can collect the active data from user
#include "../UserDataBase/UserDataBase.hpp"
#include "../DataCollector/DataCollector.hpp"
#include "../../MessagePackage/MessagePackage.hpp"

class HosterServer {

private:

    std::unique_ptr<UserDataBase> m_user_data_base;

    // the Tcp socket class
    std::unique_ptr<TCPServer> m_tcp_server;

    // recv & accept client thread
    bool m_thread_stop = false;
    std::unique_ptr<std::thread> m_listen_thread;

    // send message to client thread
    std::unique_ptr<std::thread> m_send_thread;

    // client content
    struct Client {

        int sockfd;

        std::string name, password;

        bool has_login = false;
        decltype(std::chrono::system_clock::now()) connect_time_point;

        explicit Client(int fd, bool login = false) {
            sockfd = fd;
            has_login = login;
        }

    };

    // store all the client that has accept
    std::vector<Client> m_clients;

    // store all the data from all client e.g. concentration
    std::unique_ptr<DataCollector> m_data_collector;

    struct ClientMessageData {

        Socket::Message mes;
        int client_fd = -1;

        ClientMessageData() = default;

        ClientMessageData(Socket::Message &&message, int fd) {
            mes = std::move(message);
            client_fd = fd;
        }
    };

    // store all the message from client
    std::deque<ClientMessageData> m_client_message;

    enum class SendType{
        BroadCast, Target
    };

    struct SendMessageData{

        Socket::Message message;
        SendType type;

        int sock_fd{};

        SendMessageData(Socket::Message &&message, SendType type, int fd) {
            this->message = message;
            this->type = type;
            sock_fd = fd;
        }

    };

    // store all the message needed to send to client
    std::deque<SendMessageData> m_send_message_queue;

    /*
     *
     *  if target_fd != nullptr, handle message only for *target_fd
     *
     */
    void MessageHandle_(const int * target_fd = nullptr);

    unsigned int m_max_client = 10;

#ifdef __linux__

    struct epoll_event ev{};
    int kdpfd, curfds = 1;

#elif defined(__WIN32__) || defined(_WIN32)


#endif

    // recv & accept client
    void StartReceive_(int max_client);

    // send message to client
    void StartSend_();

    void ResetClient_(Client& client);

    //screen shot
    decltype(std::chrono::system_clock::now()) m_screenshot_timer;
    ScreenShot m_screenShot;

public:

    explicit HosterServer(unsigned int max_client);
    ~HosterServer();

    void Start();

    void Update();
};


#endif //EMCMEETING_HOSTERSERVER_HPP
