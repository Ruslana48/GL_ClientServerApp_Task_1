#include <iostream>
#include "Network/TcpSocketNetwork.h"
#include "Socket/Socket.h"
#include "Server/Server.h"
#include "Server/Server.cpp"
#include "/home/ruslana/ClientServerApp/Client/Client.h"
#include "Client/Client.cpp"
#include <string>

int main(){
    const std::string ip="127.0.0.1";
    unsigned int port=80800;
    application::Server server(ip, port);
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
            std::cout<<"problem in main\n";
        }
    }
    
}