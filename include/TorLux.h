#ifndef INCLUDE_TORLUX
#define INCLUDE_TORLUX

#include <atomic>
#include <csignal>
#include <vector>
#include <string>
#include <pthread.h>

class TorLux {
public:
    TorLux() = delete;
    ~TorLux() = delete;

    static void run(bool mode, const char *token = nullptr);

    static std::atomic<bool> exitFlag;
    static uint8_t initcode[32], chatcode[32];
    
    static pthread_mutex_t chatMutex;
    static std::vector<std::string> chatBuffer;

    static pthread_t senderThread, serverThread;
private:
    static void parseToken(const char *token);
    static void initiate();
    static void join();
};

#endif /* INCLUDE_TORLUX */
