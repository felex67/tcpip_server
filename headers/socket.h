#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>


namespace ftwd {
    class Socket {
        typedef unsigned char byte;

        static const byte flagOff = 0;
        static const byte cntrOff = 1;
        static const byte fdOff = cntrOff + sizeof(size_t);
        static const byte arrSize = cntrOff + sizeof(int) + sizeof(size_t);
        
        byte* _data;

        public:

        Socket(int fd)
            : _data(nullptr)
        {
            _data = new byte[arrSize];
            *_data = true;
            *(size_t*)(_data + cntrOff) = 1;
            fcntl(fd, F_SETFD, O_NONBLOCK);
            *(int*)(_data + fdOff) = fd;
            *_data = false;
        }
        ~Socket() {
            while(*_data);
            *_data = true;
            if(!(--(*(size_t*)(_data + cntrOff)))) {
                close(*(int*)(_data + fdOff));
                delete[] _data;
            }
        }
        Socket(Socket &other)
            : _data(nullptr)
        {
            while(*(other._data));
            *(other._data) = true;
            _data = other._data;
            ++(*(size_t*)(_data + cntrOff));
            *_data = false;
        }

    };
};