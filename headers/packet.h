#ifndef _packet_h_
#define _packet_h_

#include <sys/types.h>
namespace ftwd {
    struct packet {
        char* pkg;
        size_t userID;
    };
};
#endif //_packet_h_