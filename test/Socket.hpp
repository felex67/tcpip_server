#ifndef _Socket_hpp_
#define _Socket_hpp_

#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <mutex>

#include "error.hpp"

class SockError : public error {
public:
    SockError(errcodes::code code);
};

namespace hidden {
    struct Socket {
        size_t reffcounter;
        int sockfd;
        struct __flags {
            int openned;
            int current;
        }      flags;
        sockaddr_in _addr;
    };
};

class Socket {
    hidden::Socket* _Socket;
    Socket();
    //true: there are some other reffs left
    bool _clear();
public:
    Socket(const int&, const sockaddr_in&);
    Socket(const Socket&);
    ~Socket();
    operator bool() const;
    errcodes::code set_nonblock();
    errcodes::code set_block();
    const sockaddr_in SockAddr() const;
    int recv(void*, size_t, int);
    int send(const void*, size_t, int);
    void close();
};

class ListenSocket {
    int sockfd;
    int NewSockFd;
    int reuseaddr;
    socklen_t cl_addrsz;
    sockaddr_in addr;
    sockaddr_in cl_addr;
public:
    ListenSocket(const unsigned short, in_addr_t iaddr = INADDR_ANY);
    ~ListenSocket();
    void init();
    Socket accept();
    operator bool() const;
};

class ClientSocket {
    hidden::Socket* _Socket;
    bool _clear();
public:
    ClientSocket(const char*, const unsigned short);
    ClientSocket(const ClientSocket&);
    ~ClientSocket();
    errcodes::code set_nonblock();
    errcodes::code set_block();
    errcodes::code connect();
    int recv(void*, size_t, int);
    int send(const void*, size_t, int);
    void close();
    operator bool() const;
};



#endif //_Socket_hpp_