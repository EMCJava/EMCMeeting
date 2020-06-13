#include "TCPServer.hpp"

//#include <utility>

int TCPServer::receive(int targetfd, Socket::Message &message) {
    // not for thread to use this function yet, no lock added.
    static constexpr auto size_info_size = sizeof(typeof(message.mes.size()));
    static std::vector<char> size_info;

    if (sockfd == EmptySock()) {
        std::cerr << "Socket fd not exist !" << std::endl;

        return -1;
    }

    int result;
    if ((result = receive(targetfd, size_info_size, size_info)) != size_info_size) {

        std::cerr << "Failed to fetch mes size" << std::endl;
        return result;

    }

    typeof(message.mes.size()) message_size;

    std::memcpy(reinterpret_cast<char *>(&message_size), size_info.data(), size_info_size);

    return receive(targetfd, message_size, message.mes);
}


int TCPServer::receive(int targetfd, size_t size, std::vector<char> &arr, float time_ous_second) {

    const clock_t begin_time = clock();

    if (arr.size() != size)
        arr.resize(size);

    typeof(arr.size()) data_received = 0;

    while (true) {
        const auto result = ::recv(targetfd, arr.data() + data_received, size - data_received, 0);

        if (result == 0) {

            std::cerr << "Socket disconnected !!" << std::endl;

            return 0;

        }

        // connect error
        if (result == -1) {

            // not out of time
            if ((float) (clock() - begin_time) / CLOCKS_PER_SEC < time_ous_second) {
                continue;
            }

            std::cerr << "Unable to receive data !!" << std::endl;
            return -1;
        }

        data_received += result;

        std::cout << "received : " << data_received << " bytes, " << size - data_received << " bytes to go."
                  << std::endl;

        if (data_received >= size) {
            break;
        }
    }

    return data_received;
}


int TCPServer::send(int targetfd, Socket::Message &message) {
    // not for thread to use this function yet, no lock added.
    static constexpr auto size_info_size = sizeof(typeof(message.mes.size()));
    static char size_info[size_info_size] = {};

    if (sockfd == EmptySock()) {
        std::cerr << "Socket fd not exist !" << std::endl;

        return -1;
    }

    const auto message_size = message.mes.size();

    std::memcpy(size_info, reinterpret_cast<const void *>(&message_size), size_info_size);
    if (::send(targetfd, size_info, size_info_size, 0) != size_info_size) {
        std::cerr << "Failed to send mes size" << std::endl;
        return -1;
    }

    typeof(message.mes.size()) message_sent = 0;

    while (true) {

        const auto result = ::send(targetfd, message.mes.data(), message.mes.size(), 0);

        // connect error
        if (result == -1) {
            std::cerr << "Unable to send data !!" << std::endl;
            return -1;
        }

        message_sent += result;

        std::cout << "sent : " << message_sent << " bytes, " << message_size - message_sent << " bytes to go."
                  << std::endl;

        if (message_sent >= message_size) {
            break;
        }
    }

    return message_sent;
}

int TCPServer::send(int targetfd, Socket::Message &&message) {
    return send(targetfd, message);
}

TCPServer::TCPServer(std::string server_ip, int port) {

    Setup(std::move(server_ip), port);
}

TCPServer::TCPServer() {
    sockfd = EmptySock();
}


void TCPServer::ShutDown() {

    if (sockfd != EmptySock()) {

        close(sockfd);

    }

}

int TCPServer::Setup(std::string server_ip, int port) {
    return Setup(inet_addr(server_ip.c_str()), port);
}

int TCPServer::Setup(in_addr_t server_ip, int port) {

    // close previous socket
    ShutDown();

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // failed to create a socket
    if (sockfd == -1) {
        std::cerr << "Fail to create a socket." << std::endl;

        sockfd = EmptySock();

        return sockfd;
    }


    int opt = 1;

    //set socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,
                   sizeof(opt)) < 0) {
        std::cerr << "setsockopt error" << std::endl;

        sockfd = EmptySock();

        return sockfd;
    }

    struct sockaddr_in serverInfo{};
    std::memset(&serverInfo, 0, sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = server_ip;
    serverInfo.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serverInfo, sizeof(serverInfo)) == -1) {

        //Failed to bind socket
        std::cerr << "Failed to bind socket." << std::endl;

        sockfd = EmptySock();
    }

    return sockfd;
}

void TCPServer::listen(int count) {

    ::listen(sockfd, count);

}

int TCPServer::accept() {

    struct sockaddr_in clientInfo{};
    int addrlen = sizeof(clientInfo);

    int ClientSockfd = ::accept(sockfd, (struct sockaddr *) &clientInfo,
                                reinterpret_cast<socklen_t *>(&addrlen));

    if (ClientSockfd == -1) {
        // Failed to connect

        std::cerr << "Failed to connect to client." << std::endl;

        ClientSockfd = EmptySock();
    }


    return ClientSockfd;
}

TCPServer::~TCPServer() {
    ShutDown();
}

TCPServer::TCPServer(int port) {
    Setup(INADDR_ANY, port);
}

int TCPServer::GetFD() {
    return Socket::GetFD();
}
