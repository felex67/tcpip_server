#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <string>
#include <list>
#include <mutex>
#include "Socket.hpp"

struct queue {
    std::mutex mutex;
    std::list<std::string> list;
};

struct _Global {
    queue in;
    queue out;
    std::mutex mDisplay;
    std::string input;
    Socket ClSocket;
} Global;

void iSockStream(_Global* global) {
    char buffer[1024];
    int len = 0;
    Socket clSocket = global->ClSocket;
    while(clSocket) {
        len = clSocket.recv(buffer, 1024, MSG_PEEK);
        if(len > 0) {
            len = clSocket.recv(buffer, len, MSG_WAITALL);
            buffer[len < 1024 ? len : 1023] = 0;
            global->in.mutex.lock();
            global->in.list.push_back(buffer);
            global->in.mutex.unlock();
        }
        else if (len == 0){
            usleep(500);
        }
        else {
            return;
        }
    }
}

void oSockStream(_Global* global) {
    Socket sock = global->ClSocket;
    queue* out = &global->out;
    while(sock) {
        out->mutex.lock();
        size_t qsize = global->out.list.size();
        if(!qsize) {
            out->mutex.unlock();
            usleep(500);
        }
        else {
            std::string t = out->list.front();
            out->list.pop_front();
            out->mutex.unlock();
            sock.send(t.c_str(), t.length(), 0);
        }
    }
}

void inputStream(_Global* global) {
    while(true) {
        std::string t;
        std::cin >> t;
        if(t[t.length() - 1] == ';') {
        }
    }
}

int main() {
    ClientSocket Socket("127.0.0.1", 7777);
    errcodes::code res = Socket.connect();
    if(res == errcodes::ERR_SUCCES) {
        std::cout << "Connected to server\n";
        std::thread _i_stream(istreamhandle, (&Socket));
        _i_stream.detach();
        while(Socket) {
            std::string s;
            std::cin >> s;
            if(s.length() > 1){
                if(s[0] == '\\' && s[1] == 'q') 
                    break;
                Socket.send(s.c_str(), s.length(), 0);
            }
        }
    }
    else {
        std::cout << "Can't connect to server...\n";
        std::cout << errcodes::emsg[res] << "\n";
        exit(1);
    }
    return 0;
}