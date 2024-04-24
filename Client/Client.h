#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <string>
#include <functional>
#include <map>
#include <dirent.h>
#include <algorithm>
#include <signal.h>
#include "/home/ruslana/ClientServerApp/Network/TcpSocketNetwork.h"

namespace application {

    struct ProcessInfo {
        std::string pid;
        std::string command;
    };

    class Client {
    private:
        network::TcpSocketNetwork socket;

    public:
        Client();
        ~Client();
        bool connect(const std::string &ip, unsigned int port);
        bool wait(const std::string &ip, unsigned int port);
        void run();

        std::vector<ProcessInfo> getProcessList();
        bool sendProcessListToServer(const std::vector<ProcessInfo>& processList);
    };

} 
