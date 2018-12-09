#ifndef _queue_h_
#define _queue_h_
/**
 * @version: 0.1b
 * @author: felexxx67@gmail.com
*/
#include <sys/types.h>
#include <mutex>

namespace ftwd {
    template <typename T>
    class list {
        
        struct pair {
            pair* pNext;
            pair* pPrev;
            T* ptr;

            pair(pair* prev, pair* next, T* _ptr)
                : pNext(prev)
                , pPrev(next)
                , ptr(_ptr)
            {}
            ~pair() {}
        };
        
        pair* pFirst;
        pair* pLast;
        size_t iSize;
        std::mutex _lock;

        static const size_t limit = 10000;
        
        public:
        list()
            : pFirst(nullptr)
            , pLast(nullptr)
            , iSize(0)
        {
            _lock.unlock();
        }
        list(const list &other);
        ~list() {
            _lock.lock();
            while(pFirst != nullptr) {
                pair* ptr = pFirst;
                pFirst = ptr->pNext;
                delete ptr;
            }
            _lock.unlock();
        }
        void push(T* ptr) {
            _lock.lock();
            if(iSize < limit) {
                pair* newPair = new pair(pLast, nullptr, ptr);
                (pLast ? (pLast->pNext = newPair) : (pFirst = newPair));
                pLast = newPair;
                ++iSize;
            }
            _lock.unlock();
        }
        void unshift(T* ptr) {
            _lock.lock();
            if(iSize < limit) {
                pair* newPair = new pair(nullptr, pFirst, ptr);
                (pFirst ? (pFirst->pPrev = newPair) : (pLast = newPair));
                pFirst = newPair;
                ++iSize;
            }
            _lock.unlock();
        }
        T* pop() {
            _lock.lock();
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
            _lock.unlock();
            return res;
        }
        T* shift() {
            _lock.lock();
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
            _lock.unlock();
            return res;
        }
        const size_t size() {
            _lock.lock();
            return iSize;
            _lock.unlock();
        }
    };
};
#endif //_queue_h_