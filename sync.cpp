#include <time.h>
#include <thread>
#include <iostream>

using namespace std;

struct mutex {
    int i;
    private:
    bool _lock;
    public:
    mutex()
        : i(0)
        , _lock(false)
    {}
    bool lock() {
        while(_lock);
        return _lock = true;
    }
    bool trylock() {
        if(_lock) {
            return false;
        }
        return _lock = true;
    }
    void unlock() {
        _lock = false;
    }
};

enum mutexes {
    RandInt,
    CmdByte,
    StateFlags,
    MutexOffset,
};

enum StateFlags {
    RndThread = 0b1,
    InputThread = 0b10,
};

mutex test[MutexOffset];

void thr_func(mutex* m) {
    unsigned int r = 0;
    srand(time(0));
    m[StateFlags].lock();
    if (!(RndThread & m[StateFlags].i)) {
        m[StateFlags].i = m[StateFlags].i | RndThread;
    }
    m[StateFlags].unlock();

    while(true) {
        char cmd = 0;
        int rnd;
        ++r;
        if (m[CmdByte].trylock()) {
            cmd = m[CmdByte].i;
            m[CmdByte].i = 0;
            m[CmdByte].unlock();
        }
        switch(cmd) {
            case('p'):
                m[RandInt].lock();
                rnd = m[RandInt].i;
                m[RandInt].unlock();
                std::cout << "Round: " << r << " Curent random int: " << rnd << "\n";
                rnd = 0;
                break;
            case('q'):
                std::cout << "Recieved finishing command, exiting process...\n";
                m[StateFlags].lock();
                m[StateFlags].i = m[StateFlags].i ^ RndThread;
                m[StateFlags].unlock();
                return;
                break;
            default:
                m[RandInt].lock();
                m[RandInt].i = rand();
                m[RandInt].unlock();
                break;
        };
    }
    return;
}

void thr_func1(mutex* test) {
    char c;
    while(true) {
        std::cin >> c;
        test[CmdByte].lock();
        test[CmdByte].i = c;
        test[CmdByte].unlock();
    }
}

int main () {
    if(!(test[StateFlags].i & RndThread)) {
        test[StateFlags].i = test[StateFlags].i | RndThread;
    }
    std::thread thr1(thr_func, test), thr2(thr_func1, test);
    thr1.detach();
    thr2.detach();
    int s = 0;
    while(true) {
        test[StateFlags].lock();
        s = test[StateFlags].i & RndThread;
        test[StateFlags].unlock();
        if(!s) break;
    }
    return 0;
}