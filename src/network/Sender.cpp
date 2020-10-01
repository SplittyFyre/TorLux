#include "Sender.h"

#include "Socks.h"
#include "TorLux.h"

std::vector<std::string> Sender::toSend;
pthread_cond_t Sender::cond;
pthread_mutex_t Sender::mutex;

void* Sender::backgroundSender(void*) {

    std::vector<char> resp;

    pthread_mutex_lock(&mutex);

    while (!TorLux::exitFlag) {
        pthread_cond_wait(&cond, &mutex);
        if (TorLux::exitFlag) break;
        for (auto &s : toSend) {
            Socks::transmit(std::vector<char>(s.begin(), s.end()), resp);
        }
        toSend.clear();
    }

    pthread_mutex_unlock(&mutex);

    return nullptr;
}