#include <cstdio>
#include <iostream>
#include <thread>
#include "headers/tslist.h"

typedef ftwd::list<char> msgQueue;

msgQueue queue;

void reader(msgQueue* ListPtr) {
    char* msg;
    while(true) {
        msg = ListPtr->pop();
        if(msg != nullptr) {
            std::cout << msg << "\n";
            delete[] msg;
            msg = nullptr;
        }
    }
}

void write(const char* msg) {
    size_t len = 0;
    while(msg[len++] != 0);
    ((len < 256) ? : len = 256);
    char* entry = new char[len];
    for(size_t i = 0; i < len; i++) {
        entry[i] = msg[i];
    }
    entry[len - 1] = 0;
    queue.unshift(entry);
}

char* rndString() {
    srand(time(0));
    size_t len = rand() % 30 + 15;
    char* str = new char[len + 1];
    for(size_t i = 0; i < len; i++) {
        str[i] = rand() % 50 + 'A';
    }
    str[len] = 0;
    return str;
}

int main(int argn, char* argv[]) {
    std::thread display(reader, &queue);
    display.detach();
    char c;
    while(true) {
        std::cin >> c;
        if(c == 'q') {
            break;
        }
        char* t = rndString();
        write(t);
        delete[] t;
    }
    return 0;
}