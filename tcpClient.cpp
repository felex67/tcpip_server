#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <string>
#include <list>
#include <mutex>

#define INVALID_SOCKET 0

struct queue {
    std::mutex mutex;
    std::list<std::string> list;
};

struct _Global {
    queue iMsgs;
    queue oMsgs;
    std::mutex display;
    std::string iBuffer;
} Global;

void mainstream(_Global* global) {
    std::string input;
    while(true) {
        while(true) {
            std::string t;
            std::cin >> t;
            input += t;
            if(input[input.length() - 1] == ';')
                break;
        }
        if(strcmp(input.c_str(), "exit")) {
            return;
        }
    }
}

int main() {
    int Socket;
    struct sockaddr_in addr;

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(Socket == INVALID_SOCKET) {
        std::cout << "Can't create Socket\n";
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(Socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cout << "Can't connect to server...\n";
        close(Socket);
        exit(1);
    }
    std::cout << "Connected to server\n";
    char buff[1024];
    while(1) {
        int msgSize = recv(Socket, buff, 1024, MSG_PEEK);
        if(msgSize > 0) {
            msgSize = recv(Socket, buff, 1024, 0);
            buff[msgSize < 1024 ? msgSize : 1023] = 0;
            std::cout << "Msg from server: '" << buff << "'\n";
        }
    }
    close(Socket);
    return 0;
}