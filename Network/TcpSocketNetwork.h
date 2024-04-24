#pragma once

#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "/home/ruslana/ClientServerApp/Socket/Socket.h"
#include <iostream>


#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

namespace network{
    class TcpSocketNetwork{
    private:

    public:
        unsigned short get_remote_port() const;

        TcpSocketNetwork();
        ~TcpSocketNetwork();
        bool connect(const std::string &ip, unsigned short port);
        void disconnect();

        bool receive(void *buffer, size_t buffer_size);
        bool receive(void *buffer, size_t buffer_size, size_t &received);
        bool send (const void *buffer, size_t buffer_size);
        bool send (void *buffer, size_t buffer_size, size_t &sent);
        const char *get_remote_address() const;

        bool create();
        socket_id m_id=INVALID_SOCKET;
        bool destroy();

        bool listen(const std::string &ip, unsigned short port);
        bool accept(TcpSocketNetwork &socket);
        void close();

        char m_remote_ip[INET_ADDRSTRLEN + 1] = {0};
        unsigned short m_remote_port = {0};

    };
}