#ifndef EMCNETWORK_TCPSERVER_HPP
#define EMCNETWORK_TCPSERVER_HPP


#include "Socket.hpp"

#include <string>

class TCPServer : protected Socket {

public:

    TCPServer(std::string server_ip, int port);

    TCPServer();
    ~TCPServer();

    // return the length that had been wrote or read
    int receive(int targetfd, Socket::Message &message) override;

    int send(int targetfd, Message &message) override;

    int send(int targetfd, Message &&message);


    void listen(int count);

    // return the socket id
    int accept();

    // return the socket id
    int Setup(std::string server_ip, int port);

    int Setup(in_addr_t server_ip, int port);

    void ShutDown();

};

#endif // EMCNETWORK_TCPSERVER_HPP