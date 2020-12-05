#ifndef INCLUDE_TORLUX
#define INCLUDE_TORLUX

#include <atomic>
#include <csignal>
#include <vector>
#include <string>
#include <pthread.h>

#define HOSTNAME_LEN 56
#define HOSTNAME_LEN_ONION (56 + 6)
#define TOKEN_LEN (32 * 2 + 56)

class TorLux {
public:
    TorLux() = delete;
    ~TorLux() = delete;

    static void run(bool mode, const char *token = nullptr);

    static volatile sig_atomic_t signalFlag;
    static std::atomic<bool> exitFlag;
    static uint8_t initcode[32], chatcode[32];
    
    static pthread_mutex_t chatMutex;
    static std::vector<std::string> chatBuffer;

    static pthread_t senderThread, serverThread;

    static std::string myAddr, targetAddr;
private:
    static void parseToken(const char *token);
    static void initiate();
    static void join();
};

#endif /* INCLUDE_TORLUX */
