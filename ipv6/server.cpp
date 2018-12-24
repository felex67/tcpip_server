#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <error.h>
#include <fcntl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


int set_nonblock(int fd) {
    int flags;
#if defined(O_NONBLOCK)
    if(-1 == (flags = fcntl(fd, F_GETFL)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}

#define EPOLL_MAX_EVENTS 32
#pragma optimization("", off)
const char* ip6tostr(const char Dest[40], sockaddr_in6& addr) {
    u_int8_t* Out = (u_int8_t*)Dest;
    u_int8_t outPos = 0;
    u_int8_t addrSize = (sizeof(addr.sin6_addr));
    u_int8_t* byteArr = (u_int8_t*)&addr.sin6_addr;
    for(u_int8_t i = 0; i < addrSize; i += 2) {
        if(i)
            Out[outPos++] = ':';
        for(int j = 1; j >= 0; j--) {
            u_int8_t c1 = byteArr[i + j] / 0x10;
            u_int8_t c2 = byteArr[i + j] % 0x10;
            c1 += ((c1 < 0xa) ? (u_int8_t)'0' : ((u_int8_t)'a' - 0xa));
            c2 += ((c2 < 0xa) ? (u_int8_t)'0' : ((u_int8_t)'a' - 0xa));
            Out[outPos] = c1;
            Out[outPos + 1] = c2;
            outPos += 2;
        }
    }
    Out[39] = 0;
    return (char*)Out;
};
#pragma optimization("", on)

int main(int argc, char** argv) {
    int ListenSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    int reuseaddr = 1;
    setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    if(!ListenSocket) {
        std::cout << "Can't create socket...\n";
    }
    /* struct sockaddr_in ServAddr;
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_port = htons(7777);
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); */

    struct sockaddr_in6 ServAddr;
    ServAddr.sin6_family = AF_INET6;
    ServAddr.sin6_port = htons(7777);
    inet_pton(AF_INET6, "::", &ServAddr.sin6_addr);
    if(bind(ListenSocket, (sockaddr*)&ServAddr, sizeof(ServAddr)) != 0) {
        std::cout << "Can't bind socket\n";
        exit(0);
    }
    set_nonblock(ListenSocket);

    listen(ListenSocket, SOMAXCONN);

    int EPoll = epoll_create1(0);

    struct epoll_event Event;
    Event.data.fd = ListenSocket;
    Event.events = EPOLLIN;
    epoll_ctl(EPoll, EPOLL_CTL_ADD, ListenSocket, &Event);

    volatile struct sockaddr_in6 ClAddr;
    u_int64_t* ptr = (u_int64_t*)&ClAddr;
    ptr[0] = ptr[1] = 0;
    socklen_t ClAddrSize;

    while(true) {
        struct epoll_event Events[EPOLL_MAX_EVENTS];
        int N = epoll_wait(EPoll, Events, EPOLL_MAX_EVENTS, -1);
        for(unsigned int i = 0; i < N; i++) {
            if(Events[i].data.fd == ListenSocket) {
                int NewClient = accept(ListenSocket, (sockaddr*)((sockaddr_in6*)&ClAddr), &ClAddrSize);
                set_nonblock(NewClient);
                struct epoll_event Event;
                Event.data.fd = NewClient;
                Event.events = EPOLLIN;
                epoll_ctl(EPoll, EPOLL_CTL_ADD, NewClient, &Event);
                usleep(50);
                char StrAddr[40];
                ip6tostr(StrAddr, *(sockaddr_in6*)&ClAddr);
                std::cout << "New connection from " << StrAddr << "\n";
            }
            else {
                static char Buffer[1024];
                int Result = recv(Events[i].data.fd, Buffer, 1024, MSG_NOSIGNAL);
                if((Result == 0) && (errno != EAGAIN)) {
                    shutdown(Events[i].data.fd, SHUT_RDWR);
                    close(Events[i].data.fd);
                    std::cout << "Client disconnected...\n";
                }
                else {
                    Buffer[Result < 1024 ? Result : 1023] = 0;
                    std::cout << "New msg: " << Buffer;
                    send(Events[i].data.fd, Buffer, Result, 0);
                }
            }
        }
    }
    return 0;
}