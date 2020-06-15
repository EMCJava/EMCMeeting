//
// Created by loyusum on 9/6/2020.
//

#ifndef EMCMEETING_USERCLIENT_HPP
#define EMCMEETING_USERCLIENT_HPP

#include <string>
#include <memory>
#include <thread>
#include <deque>
#include <mutex>

#include "../../../ToolBox/Constant.hpp"
#include "../AccountData/AccountData.hpp"

#include "../Hoster/DataCollector/DataCollector.hpp"
#include "../MessagePackage/MessagePackage.hpp"

#include "../../../ToolBox/ToolBox.hpp"

#include "../../EMCMeeting.hpp"

// Network
#include <sys/epoll.h>
#include "../NetWork/TCPClient.hpp"


class UserClient {

private:

    EMCMeeting* m_meeting_core = nullptr;

    // send & recv & accept client thread
    bool m_listen_stop = false;
    std::unique_ptr<std::thread> m_listen_thread;

    std::mutex m_listen_thread_mutex;

    // the Tcp client class, the connection between hoster and user
    std::unique_ptr<TCPClient> m_tcp_client;

    bool m_has_connect_server = false;

    // store all the message from server
    std::deque<Socket::Message> m_server_message;

    /*
     *
     *  loop to recv message from server till m_listen_stop equal true
     *
     */
    void Start_();

    void MessageHandle_();
public:

    UserClient(std::string server_ip, int port);
    ~UserClient();

    bool HasConnect();

    /*
     *
     * return true if successfully login to the server
     *
     */
    bool login(AccountData &accountData);

    /*
     *
     *  Start a thread to recv all message from server
     *
     */
    void Start();

    void logout();

    /*
    *
    *  Send attention data to server
    *
    */
    void SendAccountDataToServer_(AccountData &accountData);

    void SetMeetingCore(EMCMeeting* emcMeeting = nullptr);

    void ListenLock();
    void ListenUnLock();

    void Update();
};


#endif //EMCMEETING_USERCLIENT_HPP
