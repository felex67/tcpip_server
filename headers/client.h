#ifndef _client_h_
#define _client_h_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace ftwd {
    struct Client {
        size_t id;
        size_t sid;
        int sockfd;
        struct sockaddr_in ClAddr;
        char name[20];
    };
};
#endif //_client_h_