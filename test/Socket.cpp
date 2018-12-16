#include <arpa/inet.h>
#include "errorcodes.hpp"
#include "Socket.hpp"

SockError::SockError(errcodes::code c) {}

Socket::Socket() {}

Socket::Socket(const int &fd, const sockaddr_in &addr) {
    _Socket = new hidden::Socket;
    char *srs = (char*)&addr, *tgt = (char*)&_Socket->_addr;
    size_t len = sizeof(addr);
    _Socket->sockfd = fd;
    for(size_t i = 0; i < len; i++) tgt[i] = srs[i];
    _Socket->reffcounter = 1;
    _Socket->flags.current = _Socket->flags.openned = fcntl(fd, F_GETFD);
}
Socket::Socket(const Socket &other) {
    if((_Socket = other._Socket) != nullptr) {
        ++(_Socket->reffcounter);
    }
}
Socket::~Socket() {
    _clear();
}
bool Socket::_clear() {
    if(_Socket != nullptr) {
        if(!(--(_Socket->reffcounter))) {
            ::close(_Socket->sockfd);
            delete _Socket;
            return true;
        }
    }
    return false;
}
Socket::operator bool() const {
    return ((_Socket != nullptr) && (_Socket->sockfd != 0) ? true : false);
}
errcodes::code Socket::set_nonblock() {
    if(_Socket != nullptr) {
        if(_Socket->sockfd != 0) {
            _Socket->flags.current = fcntl(_Socket->sockfd, F_SETFD, O_NONBLOCK);
            return errcodes::ERR_SUCCES;
        }
        return errcodes::SOCK_INVALIDFD;
    }
    return errcodes::SOCK_NULLPTR;
}
errcodes::code Socket::set_block() {
    if(_Socket != nullptr) {
        if(_Socket->sockfd != 0) {
            _Socket->flags.current = fcntl(_Socket->sockfd, F_SETFD, _Socket->flags.openned);
            return errcodes::ERR_SUCCES;
        }
        return errcodes::SOCK_INVALIDFD;
    }
    return errcodes::SOCK_NULLPTR;
}
int Socket::recv(void* ibuff, size_t buff_sz, int flags) {
    int recieved = ::recv(_Socket->sockfd, ibuff, buff_sz, flags);
    return recieved;
}
int Socket::send(const void* obuff, size_t buff_sz, int flags) {
    int sended = ::send(_Socket->sockfd, obuff, buff_sz, flags);
    return sended;
}
void Socket::close() {
    if(_Socket && _Socket->sockfd) {
        _Socket->flags.current = fcntl(_Socket->sockfd, F_SETFD, _Socket->flags.openned);
        ::close(_Socket->sockfd);
        _Socket->sockfd = 0;
    }
}


void ListenSocket::init() {
    if((sockfd = socket(addr.sin_family, SOCK_STREAM, IPPROTO_TCP)) == 0) {
        throw SockError(errcodes::SOCK_CREATE);
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr))) {
        close(sockfd);
        sockfd = 0;
        throw SockError(errcodes::SOCK_SETOPT);
    }
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        close(sockfd);
        sockfd = 0;
        throw SockError(errcodes::SOCK_BIND);
    }
    if(listen(sockfd, SOMAXCONN) < 0) {
        close(sockfd);
        sockfd = 0;
        throw SockError(errcodes::SOCK_LISTEN);
    }
}
ListenSocket::ListenSocket(const unsigned short _PortNo, in_addr_t iaddr)
    : sockfd(0)
    , NewSockFd(0)
    , reuseaddr(1)
    , cl_addrsz(0)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_PortNo);
    addr.sin_addr.s_addr = iaddr;
}
ListenSocket::~ListenSocket() {
    if(sockfd) {
        close(sockfd);
    }
}
Socket ListenSocket::accept() {
    if(sockfd) {
        NewSockFd = ::accept(sockfd, (sockaddr*)&cl_addr, &cl_addrsz);
        return Socket(NewSockFd, cl_addr);
    }
}
ListenSocket::operator bool() const {
    return (sockfd ? true : false);
}

int ClientSocket::recv(void* ibuff, size_t buff_sz, int flags) {
    int recieved = ::recv(_Socket->sockfd, ibuff, buff_sz, 0);
    return recieved;
}
int ClientSocket::send(const void* obuff, size_t buff_sz, int flags) {
    int sended = ::send(_Socket->sockfd, obuff, buff_sz, 0);
    return sended;
}
void ClientSocket::close() {
    if(_Socket && _Socket->sockfd) {
        _Socket->flags.current = fcntl(_Socket->sockfd, F_SETFD, _Socket->flags.openned);
        ::close(_Socket->sockfd);
        _Socket->sockfd = 0;
    }
}

ClientSocket::ClientSocket(const char* strAddres, const unsigned short PortNo)
    : _Socket(nullptr)
{
    _Socket = new hidden::Socket;
    _Socket->sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    _Socket->reffcounter = 1;
    _Socket->_addr.sin_family = AF_INET;
    _Socket->_addr.sin_port = htons(PortNo);
    _Socket->_addr.sin_addr.s_addr = inet_addr(strAddres);
}
ClientSocket::ClientSocket(const ClientSocket& other) {
    if((_Socket = other._Socket) != nullptr) {
        ++(_Socket->reffcounter);
    }
}
ClientSocket::~ClientSocket() {
    _clear();
}
errcodes::code ClientSocket::set_nonblock() {
    if(_Socket != nullptr) {
        if(_Socket->sockfd != 0) {
            _Socket->flags.current = fcntl(_Socket->sockfd, F_SETFD, O_NONBLOCK);
            return errcodes::ERR_SUCCES;
        }
        return errcodes::SOCK_INVALIDFD;
    }
    return errcodes::SOCK_NULLPTR;
}
errcodes::code ClientSocket::set_block() {
    if(_Socket != nullptr) {
        if(_Socket->sockfd != 0) {
            _Socket->flags.current = fcntl(_Socket->sockfd, F_SETFD, _Socket->flags.openned);
            return errcodes::ERR_SUCCES;
        }
        return errcodes::SOCK_INVALIDFD;
    }
    return errcodes::SOCK_NULLPTR;
}
bool ClientSocket::_clear() {
    if(_Socket != nullptr) {
        if(!(--_Socket->reffcounter)) {
            ::close(_Socket->sockfd);
            delete _Socket;
            return true;
        }
    }
    return false;
}
errcodes::code ClientSocket::connect() {
    if (::connect(_Socket->sockfd, (struct sockaddr *)&(_Socket->_addr), sizeof(_Socket->_addr)) < 0) {
        ::close(_Socket->sockfd);
        return errcodes::SOCK_CONNECT;
    }
    return errcodes::ERR_SUCCES;
}
ClientSocket::operator bool() const {
    return ((_Socket != nullptr) && (_Socket->sockfd != 0) ? true : false);
}