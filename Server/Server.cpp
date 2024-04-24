#include "Server.h"
#include "/home/ruslana/ClientServerApp/Network/TcpSocketNetwork.cpp"

namespace application {

    Server::Server(){
        std::cout<<"Hello from server";
    }

    Server::Server(const std::string& ip, unsigned int port){
        if (!m_listener.listen(ip, port)) {
            return;
            std::cout<<"listener bad\n";
        }
        std::cout<<"listener true\n";
    }

    Server::~Server() {
        stop();
    }

    void Server::run() {
        std::cout << "Server running" << std::endl;

        while (true) {
            network::TcpSocketNetwork clientSocket;

            if (!m_listener.accept(clientSocket)) {
                std::cerr << "Failed to accept connection from client" << std::endl;
                break;
            }

            std::cout << "Accepted connection from: " << clientSocket.get_remote_address() << ":" << clientSocket.get_remote_port() << std::endl;

            std::string request = "get_processes";
            if (!clientSocket.send(request.c_str(), request.size())) {
                std::cerr << "Failed to request process list from client" << std::endl;
                continue;
            } else {
                std::cout << "Request sent\n";
            }

            char buffer[1024];
            size_t received;
            if (!clientSocket.receive(buffer, sizeof(buffer), received)) {
                std::cerr << "Failed to receive process list from client" << std::endl;
                continue;
            }

            std::cout << "Process list received from client:" << std::endl;

            std::string receivedData(buffer, received);
            std::cout<<"received data length"<<receivedData.length();
            std::istringstream iss(receivedData);
            std::string pid, command;
            while (iss >> pid >> command) {
                std::cout << "PID: " << pid << ", Command: " << command << std::endl;
            }

            //send request to close process
            std::string requestOfClosing = "close_processes ";
            int pidToChoose=0;
            std::cout<<"Enter pid which you want to close\n";
            std::cin>>pidToChoose;
            std::stringstream ss;
            ss << pidToChoose;
            std::string pidString = ss.str();
            requestOfClosing+=pidString;
            if (!clientSocket.send(requestOfClosing.c_str(), requestOfClosing.size())) {
                std::cerr << "Failed to request process closing" << std::endl;
                continue;
            } else {
            std::cout << "Request sent\n";
            }
        }
    }

    void Server::stop() {
        m_listener.close();
    }

} 
