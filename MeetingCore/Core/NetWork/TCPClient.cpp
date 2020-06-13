//
// Created by loyusum on 6/6/2020.
//

#include "TCPClient.hpp"

int TCPClient::receive(Socket::Message &message) {
    if(!connected_){
        std::cerr << "Client not connected !" << std::endl;

        return -1;
    }

    return TCPServer::receive(sockfd, message);
}

int TCPClient::send(Socket::Message &message) {
    if(!connected_){
        std::cerr << "Client not connected !" << std::endl;

        return -1;
    }

    return TCPServer::send(sockfd, message);
}

int TCPClient::send(Socket::Message &&message) {
    return send(message);
}

TCPClient::TCPClient() : TCPServer() {
    SetUp();
}

TCPClient::TCPClient(std::string server_ip, int port) {
    SetUp();
    connect(server_ip, port);
}

int TCPClient::connect(std::string server_ip, int port) {
    return connect(inet_addr(server_ip.c_str()), port);
}

int TCPClient::connect(in_addr_t server_ip, int port) {

    struct sockaddr_in info{};
    std::memset(&info, 0, sizeof(info));

    info.sin_family = PF_INET;
    info.sin_addr.s_addr = server_ip;//IP address
    info.sin_port = htons(port);

    if (::connect(sockfd, (struct sockaddr *) &info, sizeof(info)) == -1) {
        std::cerr << "Connection error" << std::endl;

        return -1;
    }

    connected_ = true;

    return 0;
}

void TCPClient::SetUp() {
    ShutDown();

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        std::cerr << "Fail to create a socket." << std::endl;

        sockfd = EmptySock();
    }
}
