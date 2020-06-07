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

        size_t max_size = 0;
        std::vector<char> mes;

    };

    virtual int receive(int targetfd, Message &message) { return -1; } // from server, bad idea
    virtual int receive(Message &message) { return -1; } // from client, bad idea

    virtual int recviceFrom(Message &message) { return -1; } // bad idea, not finish

    virtual int send(int targetfd, Message &message) { return -1; } // from server, bad idea
    virtual int send(Message &message) { return -1; } // from client, bad idea

    static constexpr int EmptySock() { return -1; }
};