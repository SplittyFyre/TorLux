#ifndef INCLUDE_SENDER
#define INCLUDE_SENDER

#include <vector>
#include <string>
#include <pthread.h>

class Sender {
public:
    Sender() = delete;
    ~Sender() = delete;
    static std::vector<std::string> toSend;
    static pthread_cond_t cond;
    static pthread_mutex_t mutex;
    static void* backgroundSender(void*);
};

#endif /* INCLUDE_SENDER */
