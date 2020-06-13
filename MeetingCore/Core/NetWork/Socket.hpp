#ifndef EMCNETWORK_SOCKET_HPP
#define EMCNETWORK_SOCKET_HPP

#include <iostream>
#include <cstring>
#include <vector>

#ifdef __linux__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // for inet_addr()
#include <unistd.h>

#else

#error Not Done Yet. Only linux

#endif

class Socket {
protected:

    int sockfd = Socket::EmptySock();

public:
    struct Message {

        std::vector<char> mes;

    };

    virtual int receive(int targetfd, Message &message) { return -1; } // from server, bad idea
    virtual int receive(Message &message) { return -1; } // from client, bad idea

    virtual int recviceFrom(Message &message) { return -1; } // bad idea, not finish

    virtual int send(int targetfd, Message &message) { return -1; } // from server, bad idea
    virtual int send(Message &message) { return -1; } // from client, bad idea

    virtual int GetFD() { return sockfd; };

    static constexpr int EmptySock() { return -1; }

    static bool IsValidIP(std::string str) {
        struct sockaddr_in sa;
        int result = inet_pton(AF_INET, str.c_str(), &(sa.sin_addr));
        return result != 0;
    };
};

#endif