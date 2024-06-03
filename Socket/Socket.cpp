#include "Socket.h"

namespace network{

    Socket::Socket(Socket &&value) noexcept:m_id(value.m_id){
        value.m_id=INVALID_SOCKET;
    }

    Socket::~Socket(){
        std::cout<<"Socket destroyed";
        destroy();
    }
    bool Socket::create(){
        m_id=socket(AF_INET, SOCK_STREAM, 0);
        if(m_id==INVALID_SOCKET){
            return 0;

        }
        return 1; // true
    }
    bool Socket::destroy(){
        if(m_id!=INVALID_SOCKET){
        }
    }
}