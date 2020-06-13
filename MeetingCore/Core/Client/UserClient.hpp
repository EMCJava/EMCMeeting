//
// Created by loyusum on 9/6/2020.
//

#ifndef EMCMEETING_USERCLIENT_HPP
#define EMCMEETING_USERCLIENT_HPP

#include <string>
#include <memory>

#include "../../../ToolBox/Constant.hpp"
#include "../AccountData/AccountData.hpp"

#include "../Hoster/DataCollector/DataCollector.hpp"

#include "../../../ToolBox/ToolBox.hpp"
#include "../NetWork/TCPClient.hpp"


class UserClient {

private:

    // the Tcp client class, the connection between hoster and user
    std::unique_ptr<TCPClient> m_tcp_client;

    bool m_has_connect_server = false;

public:

    UserClient(std::string server_ip, int port);

    bool HasConnect();

    /*
     *
     * return true if successfully login to the server
     *
     */
    bool login(AccountData &accountData);

    void logout();

    /*
    *
    *  Send attention data to server
    *
    */
    void SendAccountDataToServer_(AccountData &accountData);

};


#endif //EMCMEETING_USERCLIENT_HPP
