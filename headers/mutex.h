#ifndef _mutex_h_
#define _mutex_h_

#include <sys/types.h>
#include <iostream>

template <typename T>
class mptr {
    typedef unsigned char byte;
    
    byte* ptr;
    
    static const byte arrsize = sizeof(size_t) + sizeof(unsigned int) + 1;
    static const byte flagOff = 0;
    static const byte refOff = 1;
    static const byte ptrOff = sizeof(unsigned int) + 1;

    const unsigned int _reffs() const {
        return *((unsigned int*)(ptr + refOff));
    }

    T* _ptr() const {
        return *((T*)(ptr + ptrOff));
    }
    void addPtr(T* ref) {
        *((T**)(ptr + ptrOff)) = ref;
    }
    void _init() {
        ptr = new byte[arrsize];
        for(char i = 0; i < arrsize; i++) {
            ptr[i] = 0;
        }
        _inc();
    }
    void _inc() {
        ++(*(unsigned int*)(ptr + 1));
    }
    bool _dec() {
        return (!(--(*(unsigned int*)(ptr + 1))));
    }
    public:
    mptr()
        : ptr(nullptr)
    {
        _init();
    }
    mptr(T* pObj)
        : ptr(nullptr)
    {
        _init();
        *(T**)(ptr + ptrOff) = pObj;
    }
    
    explicit mptr(const mptr<T> &other)
        : ptr(other.ptr)
    {
        _inc();
    }
    
    ~mptr() {
        if(_dec()) {
            if(*(T**)(ptr + ptrOff) != nullptr) {
                delete *((T**)(ptr + ptrOff));
            }
            delete[] ptr;
        }
    }

    T* lock() {
        while (*ptr);
        *ptr = true;
        return *(T**)(ptr + ptrOff));
    }
    
    T* trylock() {
        if(!(*ptr)) {
            *ptr = true;
            return *(T**)(ptr + ptrOff));
        }
        return nullptr;
    }

    void unlock() {
        *ptr = false;
        return;
    }

    T* release() {
        T* p = *(T**)(ptr + ptrOff);
        *(T**)(ptr + ptrOff) = 0;
        return p;
    }
    T& operator*() {
        return (*(*(T**)(ptr + ptrOff)));
    }
    T* operator->(){
        return *(T**)(ptr + ptrOff);
    }
};

#endif //_mutex_h_