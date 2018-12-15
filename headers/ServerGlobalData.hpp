#pragma once
#ifndef _ServerGlobalData_h_
#define _ServerGlobalData_h_

#include <sys/types.h>
#include <sys/io.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "tslist.hpp"
#include "client.hpp"
#include "packet.hpp"

/**
 * @version: 1.0
 * @author: felexxx67@gmail.com
*/
//class ServerCore;
namespace ftwd {
    struct ServerGlobals {
        enum thrFlags {
            SockListener =      0b0000000000000001,
            SockHandler =       0b0000000000000010,
            SockReader =        0b0000000000000100,
            SockWriter =        0b0000000000001000,
            Console =           0b0000000000010000,
        };
        short ThrFlags;

        list<Client*> Clients;
        list<packet*> packetsOut;
        list<packet*> packetsIn;
        list<char*> ConsMsg;
        list<char*> UserCmd;
        
    };
};

#endif //!_ServerGlobalData_h_