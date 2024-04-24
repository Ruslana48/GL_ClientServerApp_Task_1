#include "Client.h"
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>

namespace application {

    Client::Client() {
    }

    Client::~Client() {
        socket.disconnect();
    }

    bool Client::connect(const std::string& ip, unsigned int port) {
        std::cout << "\nConnecting to the server at " << ip << ":" << port << std::endl;
        if (socket.connect(ip, port)) {
            std::cout << "Connected successfully" << std::endl;
            return true;
        } else {
            std::cerr << "Failed to connect to the server" << std::endl;
            return false;
        }
    }

    bool Client::wait(const std::string& ip, unsigned int port) {
        network::TcpSocketNetwork listener;
        if (listener.listen(ip, port)) {
            if (listener.accept(socket)) {
                std::cout << "Accepted connection from client" << std::endl;
                return true;
            } else {
                std::cerr << "Failed to accept connection from client" << std::endl;
                return false;
            }
        } else {
            std::cerr << "Failed to start listening for connections" << std::endl;
            return false;
        }
    }

    std::vector<ProcessInfo> Client::getProcessList() {
        std::vector<ProcessInfo> processList;

        DIR* procDir = opendir("/proc");
        if (!procDir) {
            std::cerr << "Error: Failed to open /proc" << std::endl;
            return processList;
        }

        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {
            if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
                std::string pid = entry->d_name;

                std::ifstream cmdlineFile("/proc/" + pid + "/cmdline");
            if (cmdlineFile.is_open()) {
                std::string command;
                std::getline(cmdlineFile, command);

                command.erase(std::remove(command.begin(), command.end(), '\0'), command.end());

                if (!command.empty()) {
                    processList.push_back({pid, command});
                }
                cmdlineFile.close();
            }
            }
        }

        closedir(procDir);
        return processList;
    }

    void Client::run() {
        char buffer[1024] = {0};
        memset(buffer, 0, sizeof(buffer));

        if (!socket.receive(buffer, sizeof(buffer))) {
            std::cerr << "Failed to receive response from server" << std::endl;
            return;
        } else {
            std::cout << "Received data from server: " << buffer << std::endl;
        }

        std::vector<ProcessInfo> processes = getProcessList();
    
        std::cout << "Process list:\n";
        for (const auto& process : processes) {
            std::cout << "PID: " << process.pid << ", Command: " << process.command << std::endl;
        }

        // Send the process list to the server
        if (!sendProcessListToServer(processes)) {
            std::cerr << "Failed to send process list to server" << std::endl;
        } else {
            std::cout << "Sent process list to server" << std::endl;
        }

        // Receive request from server to delete specific process
        std::string closeRequest;
        if (!socket.receive(buffer, sizeof(buffer))) {
            std::cerr << "Failed to receive close process request from server" << std::endl;
        } else {
            closeRequest = buffer;
            std::cout << "Received close process request from server: " << closeRequest << std::endl;
            std::istringstream iss(closeRequest);
            std::string command, pidStr;
            iss >> command >> pidStr;
            if (command == "close_processes") {
                int pidToClose = std::stoi(pidStr);
                if (kill(pidToClose, SIGTERM) == -1) {
                    std::cerr << "Error closing process with PID " << pidToClose << ": " << strerror(errno) << std::endl;
                } else {
                    std::cout << "Closed process with PID: " << pidToClose << std::endl;
                }
            } else {
                std::cerr << "Invalid close process request from server" << std::endl;
            }
        }
    }


    bool Client::sendProcessListToServer(const std::vector<ProcessInfo>& processList) {
        std::stringstream ss;
        for (const auto& process : processList) {
            ss << process.pid << " " << process.command << "\n";
        }
        std::string serializedProcessList = ss.str();

        if (!socket.send(serializedProcessList.c_str(), serializedProcessList.length())) {
            return false;
        } else {
            return true;
        }
    }
} 
