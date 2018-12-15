#include <unistd.h>
#include <cstddef>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <mutex>
#include "errors.hpp"

namespace ftwd {    
    class Socket {
        /* helpers */
        struct _flags {
            int onCreate;
            int Current;
        };
        struct _SockInfo {
            _flags flags;
            int SockFD;
            size_t Reffs;
            sockaddr_in6 SockAddr;
        };
        /*-----------------*/
        //Указатель на динамическую память
        _SockInfo* sockdata;
        
        void _clear() {
            if(sockdata) {
                if(--(sockdata->Reffs)) {
                    sockdata = nullptr;
                }
                else {
                    fcntl(sockdata->SockFD, F_SETFD, sockdata->flags.onCreate);
                    close(sockdata->SockFD);
                    delete sockdata;
                }
            }
        }
        ushort is_nonblock() {
            if(sockdata)
                return ((sockdata->flags.Current & O_NONBLOCK) == O_NONBLOCK);
            return false;
        }
        bool set_nonblock() {
            if(sockdata) {
                if((sockdata->flags.Current & O_NONBLOCK) != O_NONBLOCK) {
                    sockdata->flags.Current = fcntl(sockdata->SockFD, F_GETFD, O_NONBLOCK);
                }
            }
            
        }
        void set_block() {
            if(sockdata) {
                if((sockdata->flags.Current & O_NONBLOCK) == O_NONBLOCK) {
                    sockdata->flags.Current = fcntl(sockdata->SockFD, F_SETFD, sockdata->flags.onCreate);
                }
            }
        }
    public:

        Socket(int fd)
            : _data(nullptr)
        {
            _data = new byte[arrSize];
            *_data = true;
            *(size_t*)(_data + CounterOffset) = 1;
            *(int*)(_data + DescriptorOffset) = fd;
            *_data = false;
        }
        ~Socket() {
            while(*_data);
            *_data = true;
            if(!(--(*(size_t*)(_data + CounterOffset)))) {
                int flags = fcntl(*(int*)(_data + DescriptorOffset), F_GETFD);
                int fd = *(int*)(_data + DescriptorOffset);
                if(fd) {
                    fcntl(*(int*)(_data + DescriptorOffset), F_SETFD, flags ^ O_NONBLOCK);
                    close(*(int*)(_data + DescriptorOffset));
                }
                delete[] _data;
            }
        }
        Socket(Socket &other)
            : _data(nullptr)
        {
            while(*(other._data));
            *(other._data) = true;
            _data = other._data;
            ++(*(size_t*)(_data + CounterOffset));
            *_data = false;
        }
        operator const int() const {
            return *(int*)(_data + DescriptorOffset);
        };
        operator int&() {
            return *(int*)(_data + DescriptorOffset);
        }
        int getFD() const {
            return *(int*)(_data + DescriptorOffset);
        }
        int& getFD() {
            return *(int*)(_data + DescriptorOffset);
        }
        operator const bool() const {
            return (*(int*)(_data + DescriptorOffset) > 0);
        }
    };
};