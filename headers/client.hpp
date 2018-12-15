#ifndef _client_h_
#define _client_h_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace ftwd {
    struct ConnClient {
        size_t id;
        size_t sid;
        int sockfd;
        struct sockaddr_in ClAddr;
    };
};
#endif //_client_h_