#ifndef _queue_h_
#define _queue_h_
/**
 * @version: 0.1b
 * @author: felexxx67@gmail.com
*/
#include <sys/types.h>

namespace ftwd {
    template <typename T>
    class list {
        
        struct pair {
            pair* pNext;
            pair* pPrev;
            T* ptr;
            char _lock;

            pair(pair* prev, pair* next, T* _ptr)
                : pNext(prev)
                , pPrev(next)
                , ptr(_ptr)
                , _lock(true)
            {}
            ~pair() {}
        };
        
        pair* pFirst;
        pair* pLast;
        size_t iSize;
        char _lock;

        static const size_t limit = 10000;
        
        public:
        list()
            : pFirst(nullptr)
            , pLast(nullptr)
            , iSize(0)
            , _lock(true)
        {
            _lock = false;
        }
        list(const list &other);
        ~list() {
            while(_lock);
            _lock = true;
            while(pFirst != nullptr) {
                pair* ptr = pFirst;
                pFirst = ptr->pNext;
                delete ptr;
            }
        }
        void push(T* ptr) {
            while(_lock);
            _lock = true;
            if(iSize < limit) {
                pair* newPair = new pair(pLast, nullptr, ptr);
                (pLast ? (pLast->pNext = newPair) : (pFirst = newPair));
                pLast = newPair;
                ++iSize;
            }
            _lock = false;
        }
        void unshift(T* ptr) {
            while(_lock);
            _lock = true;
            if(iSize < limit) {
                pair* newPair = new pair(nullptr, pFirst, ptr);
                (pFirst ? (pFirst->pPrev = newPair) : (pLast = newPair));
                pFirst = newPair;
                ++iSize;
            }
            _lock = false;
        }
        T* pop() {
            while(_lock);
            _lock = true;
            T* res = nullptr;
            if(iSize) {
                pair* ptr = pLast;
                pLast = pLast->pPrev;
                res = ptr->ptr;
                ptr->ptr = nullptr;
                delete ptr;
                (pLast ? : pFirst = nullptr);
                --iSize;
            }
            if(pLast != nullptr) {}
            else {
                pFirst = nullptr;
            }
            _lock = false;
            return res;
        }
        T* shift() {
            while(_lock);
            _lock =  true;
            T* res = nullptr;
            if(iSize) {
                pair* ptr = pFirst;
                pFirst = pFirst->pNext;
                res = ptr->ptr;
                ptr->ptr = nullptr;
                delete ptr;
                (pFirst ? : pLast = nullptr);
                --iSize;
            }
            _lock = false;
            return res;
        }
        const size_t size() {
            while(_lock);
            _lock = true;
            return iSize;
            _lock = false;
        }
    };
};
#endif //_queue_h_