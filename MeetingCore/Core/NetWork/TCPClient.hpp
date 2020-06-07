//
// Created by loyusum on 6/6/2020.
//

#ifndef EMCNETWORK_TCPCLIENT_HPP
#define EMCNETWORK_TCPCLIENT_HPP

#include "TCPServer.hpp"

class TCPClient : protected TCPServer{

private:
    bool connected_ = false;

public:
    TCPClient(std::string server_ip, int port);
    TCPClient();

    int receive(Socket::Message &message) ;
    int send(Message &message) ;
    int send(Message &&message);

    void connect(std::string server_ip, int port);
    void connect(in_addr_t server_ip, int port);

    void SetUp();
};


#endif //EMCNETWORK_TCPCLIENT_HPP
