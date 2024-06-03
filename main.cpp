#include <iostream>
#include "Network/TcpSocketNetwork.h"
#include "Socket/Socket.h"
#include "Server/Server.h"
#include "Server/Server.cpp"
#include "Client.h"
#include "Client/Client.cpp"
#include <string>

int main(){
    const std::string ip="127.0.0.1"; // read from args
    unsigned int port=80800;
    application::Server server(ip, port); // It is better two have 2 different binaries
    std::cout<<"Write S or C\n";
    char input=std::cin.get();
    application::Client client;
    if(input=='S'){
        server.run();
    }
    else{
        if(client.connect(ip, port)){
            client.run();
        }else{
            std::cout<<"problem in main\n"; // Log doesn't provide useful information
        }
    }
    
}