#include "Thread/Thread.h"

namespace system{
    Thread::Thread(){

    }
    Thread::Thread(Thread &&value){
        thread=value.thread;
        value.thread=pthread_t();
        th_joinable=value.th_joinable;
        value.th_joinable=0;

        can_destroy=value.can_destroy;
        value.can_destroy=0;
    }
    Thread::~Thread(){
        if(can_destroy){
            pthread_cancel(thread);
        }
    }

    Thread::Thread(void *(*func)(void *), void *params){
        int tmp=pthread_create(&thread, nullptr,
         func, params);
         if(tmp){
            std::cout<<"Error of creating thread\n";
         }
         can_destroy=1;
         th_joinable=1;
    }
    bool Thread::joinable() const{
        return joinable;
    }
    bool Thread::join(){
        if(pthread_join(thread, nullptr)){
            return 0;
        }
        th_joinable=0;
        can_destroy=0;
        return 1;
    }
}