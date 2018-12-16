#pragma once

namespace errcodes {
    enum code {
        ERR_SUCCES,
        SOCK_NULLPTR,
        SOCK_INVALIDFD,
        SOCK_CREATE,
        SOCK_SETOPT,
        SOCK_BIND,
        SOCK_LISTEN,
        SOCK_CONNECT,
        ERR_ENDOFLIST
    };
    const char* const emsg[ERR_ENDOFLIST] = {
        "Succes",
        "Socket.cpp: Передан нулевой указатель",
        "Socket.cpp: Неверный дескриптор файла",
        "Socket.cpp: Ошибка создания сокета",
        "Socket.cpp: Ошибка применения опций к сокету",
        "Socket.cpp: Ошибка привязки сокета к адресу",
        "Socket.cpp: Ошибка прослушки",
        "Socket.cpp: Ошибка подключения к серверу"
    };
};
