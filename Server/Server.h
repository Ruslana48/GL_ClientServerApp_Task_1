#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>
#include <netinet/in.h>
#include "TcpSocketNetwork.h"

namespace application{
    class Server{
        public:
            Server();
            Server(const std::string &ip, unsigned int port);
            ~Server();

            // Should be not copyable
            Server(const Server&) = delete;
            Server& operator=(const Server&) = delete;

            void run();
            void stop();

        private:
            network::TcpSocketNetwork m_listener;
    };
}