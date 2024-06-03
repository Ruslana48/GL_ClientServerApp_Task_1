#pragma once
#include <iostream>
#include <pthread.h>

namespace system{
    class Thread{ // Not used at all
        private:
            pthread_t thread;
            bool th_joinable=0;
            bool can_destroy=0;
        public:
            Thread();
            ~Thread();
            Thread(void *(*func)(void *), void *params);
            Thread(const Thread &val)=delete;
            Thread(Thread &&value);
            bool joinable() const;
            bool join();
    };
}
