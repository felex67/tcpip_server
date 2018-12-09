#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h>
#include "headers/tslist.h"

enum sock_opt {
    MaxClients = 50
};

struct msg {
    char* msg;
    int sockfd;
};

struct Client {
    int sockfd;
    int id;
    Client()
        :sockfd(0)
        ,id(0)
    {}
};

struct packet {
    int size;
    char* buffer;
};

Client clients[MaxClients + 1];
std::mutex mClients();

ftwd::list<msg> MsgList;

int Sockets[MaxClients + 1];

void listener(Client* pClients) {
    Client* listen = pClients + MaxClients;
    if((listen->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Cant create listen socket...\n");
        close(listen->sockfd);
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listen->sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Can't bind server socket...\n");
        close(listen->sockfd);
        exit(2);
    }

    while(true) {
        int newSock = accept(listen->sockfd, 0, 0);
        int i = 0;
        if(newSock >= 0){
            for(; i < MaxClients; i++) {
                if(pClients[i].sockfd == 0) {
                    pClients[i].sockfd = newSock;
                    pClients[i].id = i;
                    fcntl(newSock, F_SETFD, O_NONBLOCK);
                    break;
                }
            }
            if(i >= MaxClients) {
                char msg[] = "Too much users...\n";
                send(newSock, msg, sizeof(msg),0);
                close(newSock);
            }   
        }
        else {
            perror("Error accept connection...\n");
        }
    }

}

int main() {

};