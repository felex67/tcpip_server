#include <unistd.h>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include "Socket.hpp"
struct Clients {
    std::mutex mutex;
    std::list<Socket> list;
};

struct queue {
    std::mutex mutex;
    std::list<std::string> msg;
};

namespace StateFlag {
    enum StateFlags {
        MAIN_THREAD = 0b00000001,
        LISTENER    = 0b00000010,
        ISTREAM     = 0b00000100,
        OSTREAM     = 0b00001000
    };
}

struct ServerData {
    size_t state;
    Clients ClList;
    queue In;
    queue Out;
} pServerData;

typedef std::list<Socket> SocketList;

void istream(ServerData* pGlobalData) {
    Clients* pClients = &pGlobalData->ClList;
    queue* in = &pGlobalData->In;
    queue* out = &pGlobalData->Out;
    while (pGlobalData->state & StateFlag::ISTREAM) {
        pClients->mutex.lock();
        if (pClients->list.size() > 0) {
            for (auto x : pClients->list) {
                char buffer[1024];
                int bytes = x.recv(buffer, 1024, MSG_PEEK);
                if(bytes > 1) {
                    int len = x.recv(buffer, bytes, 0);
                    std::cout << buffer << "\n";
                    out->mutex.lock();
                    buffer[((len < 1024) ? len : 1023)] = 0;
                    out->msg.push_back(std::string(buffer));
                    out->mutex.unlock();
                }
                else if(bytes == 0);
                else {
                    x.close();
                    pClients->list.remove(x);
                }
            }
        }
        pClients->mutex.unlock();
        usleep(500);
    }
    if(pGlobalData->state & StateFlag::ISTREAM) {
        pGlobalData->state ^= StateFlag::ISTREAM;
    }
    return;
}
void ostream(ServerData* pGlobalData) {
    Clients* pClients = &pGlobalData->ClList;
    queue* out = &pGlobalData->Out;
    while (pGlobalData->state & StateFlag::OSTREAM) {
        out->mutex.lock();
        if(out->msg.size()) {
            std::string msg(out->msg.front());
            int len = msg.length();
            const char* string = msg.c_str();
            out->msg.pop_front();
            out->mutex.unlock();
            pClients->mutex.lock();
            for(auto x : pClients->list) {
                x.send(string, len, 0);
            }
            pClients->mutex.unlock();
        }
        else {
            out->mutex.unlock();
            usleep(500);
        }
    }
    if(pGlobalData->state & StateFlag::OSTREAM) {
        pGlobalData->state ^= StateFlag::OSTREAM;
    }
}
int main() {
    ListenSocket Listener(7777, INADDR_ANY);
    try {
        Listener.init();
    } catch (SockError e) {
        std::cout << "Some error hapend: " << errcodes::emsg[e] << "\n";
        exit(1);
    }
    
    std::thread thrIstream(istream, &pServerData);
    std::thread thrOstream(ostream, &pServerData);
    thrIstream.detach();
    thrOstream.detach();
    std::string hello("<html>Hello Client! =)</html>");
    
    while (Listener) {
        Socket newClient = Listener.accept();
        std::cout << "New Client connected...\n";
        newClient.set_nonblock();
        newClient.send(hello.c_str(), hello.length(), 0);
        pServerData.ClList.mutex.lock();
        pServerData.ClList.list.push_back(newClient);
        pServerData.ClList.mutex.unlock();
    }
    return 0;
}