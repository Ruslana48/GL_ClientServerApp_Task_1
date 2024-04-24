#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>

typedef int socket_id;

#define INVALID_SOCKET -1

namespace network{
    class Socket{
        public:
            Socket()=default;
            Socket(Socket &&value) noexcept;
            virtual ~Socket();

            socket_id m_id=INVALID_SOCKET;
        
            virtual bool create();
            bool destroy();

    };
}
