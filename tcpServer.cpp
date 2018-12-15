#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <fcntl.h>
#include <iostream>
#include <mutex>

#include "headers/tslist.hpp"

#define INVALID_SOCKET 0
#if SOMAXCONN < 1024
    #undef SOMAXCONN
    #define SOMAXCONN 1024
#endif


void displayBinary(void* ptr, size_t size, bool prefix = true) {
    char* cptr;
    if(prefix)
        std::cout << "0b";
    for(size_t i = 0; i < size; i++) {
        for(size_t j = 0; j < 8; j++) {
            std::cout << (int)(cptr[i] & (1 << j));
        }
    }
    return ;
}



struct ClientSocket {
    int Socket;
    struct __flags {
        int openned;
        int current;
    }      flags;
    struct sockaddr_in addr;
    ClientSocket() : Socket(0) {}
    ~ClientSocket() {
        if(Socket) {
            close(Socket);
        }   
    }
};

struct chatUser : public ClientSocket {
    char* name;
    size_t id;
    std::mutex _lock;
    ClientSocket* pSocket;
};

struct __ConnData {
    std::mutex _lock;
    ClientSocket clients[SOMAXCONN];
    size_t Users;
};
__ConnData* ConnData;

size_t getFreeSID() {
    ConnData->_lock.lock();
    for(size_t i = 0; i < SOMAXCONN; i++) {
        if(ConnData->clients[i].Socket == 0)
            return i;
    }
    ConnData->_lock.unlock();
    return SOMAXCONN;
};

void sockhandler(int ClSocket, sockaddr_in6 ClAddres) {
    size_t sid = getFreeSID();
    ClientSocket* clients = ConnData->clients;
    int openned = fcntl(ClSocket, F_GETFD);
    int current = fcntl(ClSocket, F_SETFD, O_NONBLOCK);
    ConnData->_lock.lock();
    displayBinary((void*)(clients[sid].flags.openned), sizeof(int));
    std::cout << "\n";
    displayBinary((void*)(clients[sid].flags.current), sizeof(int));
    close(clients[sid].Socket);
    clients[sid].Socket = 0;
    char buff[1024];
    recv(ClSocket, buff, 1024,0);
    std::cout << buff << "\n";
    send(ClSocket, "Hello Client!!!", 16, 0);
    close(ClSocket);
    return;
}


int main() {
    int ServerSocket, ClientSocket;
    int reuseaddr = 1;
    struct sockaddr_in addr;

    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ServerSocket == INVALID_SOCKET) {
        std::cout << "Can't create Socket\n";
        exit(1);
    }
    if(setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr))) {
        std::cout << "can't set socket options\n";
        close(ServerSocket);
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int err = bind(ServerSocket, (struct sockaddr*)&addr, sizeof(addr));
    if(err != 0) {
        std::cout << "Can't bind socket\n";
        close(ServerSocket);
        exit(1);
    }
    if(listen(ServerSocket, SOMAXCONN) < 0) {
        std::cout << "Listen failed...\n";
        close(ServerSocket);
        exit(1);
    }
    struct sockaddr_in6 ClAddr;
    socklen_t ClAddrLen;
    char c;
    while(true) {
        std::cout << "Waiting for users...\n";
        ClientSocket = accept(ServerSocket, (sockaddr*)&ClAddr, &ClAddrLen);
        if(ClientSocket != INVALID_SOCKET) {
            std::cout << "Incoming connection\n";
            sockhandler(ClientSocket, ClAddr);
        }
    }
    close(ServerSocket);
    return 0;
}