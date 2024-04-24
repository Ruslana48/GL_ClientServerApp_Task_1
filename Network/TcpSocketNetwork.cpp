#include "TcpSocketNetwork.h"

namespace network{
    TcpSocketNetwork::TcpSocketNetwork() : m_id(INVALID_SOCKET), m_remote_port(0) {}

    TcpSocketNetwork::~TcpSocketNetwork(){
        close();
    }

    bool TcpSocketNetwork::connect(const std::string &ip, unsigned short port){
        if (!create()){
            return false;
        }

        memcpy(m_remote_ip, ip.c_str(), std::min(sizeof(m_remote_ip), ip.length() + 1));
        m_remote_port = port;
        sockaddr_in hint = {0};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_remote_port);
        inet_pton(AF_INET, m_remote_ip, &hint.sin_addr);
        if (::connect(m_id, reinterpret_cast<sockaddr *>(&hint), sizeof(hint)) == SOCKET_ERROR){
            std::cout<<"connect false\n";
            return false;
        }
        return true;
    }
    

    bool TcpSocketNetwork::accept(TcpSocketNetwork &socket)
    {
        sockaddr_in hint = {0};
        socklen_t len = sizeof(hint);
        auto val = ::accept(m_id, (struct sockaddr *)&hint, &len);
        if (val == SOCKET_ERROR)
        {
            std::cout<<"accept failed "<<strerror(errno)<<std::endl;
            return 0;
        }
        socket.m_id = val;
        inet_ntop(AF_INET, &(hint.sin_addr), &(socket.m_remote_ip[0]), INET_ADDRSTRLEN);
        socket.m_remote_port = ntohs(hint.sin_port);

        return 1;
    }

    bool TcpSocketNetwork::listen(const std::string &ip, unsigned short port){
        if (!create())
            return false;

        sockaddr_in info;

        info.sin_family = AF_INET;
        info.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &info.sin_addr);
        
        if (bind(m_id, reinterpret_cast<sockaddr *>(&info), sizeof(info)) == SOCKET_ERROR){
            return false;
        }

        if (::listen(m_id, SOMAXCONN) == SOCKET_ERROR){
            return false;
        }
        return true;
    }


    bool TcpSocketNetwork::receive(void *buffer, size_t buffer_size) {
        std::cout << "Receiving data..." << std::endl;
        size_t bytes_received = recv(m_id, buffer, buffer_size, 0);
        if (bytes_received == -1) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            return false;
        } else if (bytes_received == 0) {
            std::cerr << "Connection closed by peer" << std::endl;
            return false;
        }
        std::cout << "Received " << bytes_received << " bytes of data." << std::endl;
        return true;
    }

    bool TcpSocketNetwork::receive(void *buffer, size_t buffer_size, size_t &received) {
        std::cout << "Receiving data..." << std::endl;
        size_t bytes_received = recv(m_id, buffer, buffer_size, 0);
        if (bytes_received == -1) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            return false;
        } else if (bytes_received == 0) {
            std::cerr << "Connection closed by peer" << std::endl;
            return false;
        }
        std::cout << "Received " << bytes_received << " bytes of data." << std::endl;
        received = static_cast<size_t>(bytes_received);
        return true;
    }

    bool TcpSocketNetwork::send(const void *buffer, size_t buffer_size) {
        std::cout << "Sending data..." << std::endl;
        ssize_t bytes_sent = ::send(m_id, buffer, buffer_size, 0);
        if (bytes_sent == -1) {
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            return false;
        }
        std::cout << "Sent " << bytes_sent << " bytes of data." << std::endl;
        return true;
    }

    bool TcpSocketNetwork::send(void *buffer, size_t buffer_size, size_t &sent) {
        std::cout << "Sending data..." << std::endl;
        ssize_t bytes_sent = ::send(m_id, buffer, buffer_size, 0);
        if (bytes_sent == -1) {
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            return false;
        }
        std::cout << "Sent " << bytes_sent << " bytes of data." << std::endl;
        sent = static_cast<size_t>(bytes_sent);
        return true;
    }


    unsigned short TcpSocketNetwork::get_remote_port() const{
        return m_remote_port;
    }

    const char *TcpSocketNetwork::get_remote_address() const{
        return m_remote_ip;
    }

    void TcpSocketNetwork::disconnect(){
        destroy();
    }

    void TcpSocketNetwork::close(){
        destroy();
    }

    bool TcpSocketNetwork::create(){
        m_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        return m_id != INVALID_SOCKET;
    }

    bool TcpSocketNetwork::destroy(){
        if (m_id != INVALID_SOCKET){
            m_id = INVALID_SOCKET;
        }
    }
}
