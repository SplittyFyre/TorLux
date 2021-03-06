#include "TorLux.h"

#include "util.h"
#include "Socks.h"
#include "UI.h"
#include "Sender.h"
#include "Server.h"
#include "Generate.h"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

volatile sig_atomic_t TorLux::signalFlag = false;
std::atomic<bool> TorLux::exitFlag(false);

uint8_t TorLux::initcode[32], TorLux::chatcode[32];

pthread_mutex_t TorLux::chatMutex;
std::vector<std::string> TorLux::chatBuffer;

pthread_t TorLux::senderThread, TorLux::serverThread;

std::string TorLux::myAddr, TorLux::targetAddr;

void TorLux::parseToken(const char *token) {
    size_t len = strlen(token);
    if (len != TOKEN_LEN) {
        puts("Invalid token");
        exit(0);
    }
    for (int i = 0; i < 32; i++) {
        int v1 = hcti(token[i * 2]), v2 = hcti(token[i * 2 + 1]);
        if (v1 == -1 || v2 == -1) {
            puts("Invalid token");
            exit(0);
        }
        initcode[i] = v1 * 16 + v2;
    }
    for (unsigned i = 64; i < len; i++) {
        char c = token[i];
        if (!validb32(c)) {
            puts("Invalid token");
            exit(0);
        }
        targetAddr.push_back(c);
    }
    targetAddr += ".onion";
}

void TorLux::run(bool mode, const char *token) {

    if (mode) {
        parseToken(token);
    }

    Generate::initRandom();
    Socks::init();
    Server::init();

    if (mode) {
        Socks::makeRequest();
        join();
    }
    else {
        initiate();
    }

    if (exitFlag) {
        Generate::cleanupRandom();
        Socks::cleanup();
        Server::cleanup();
        puts("Received SIGINT, exiting");
        return;
    }

    UI::init();

    pthread_create(&serverThread, nullptr, Server::run, nullptr);
    pthread_create(&senderThread, nullptr, Sender::backgroundSender, nullptr);

    while (true) {
        if (signalFlag) {
            exitFlag = true;
            break;
        }

        if (!UI::update()) {
            exitFlag = true;
            break;
        }
    }

    pthread_join(serverThread, nullptr);

    pthread_mutex_lock(&Sender::mutex);
    pthread_cond_signal(&Sender::cond);
    pthread_mutex_unlock(&Sender::mutex);
    pthread_join(senderThread, nullptr);

    Generate::cleanupRandom();
    Socks::cleanup();
    Server::cleanup();
    UI::cleanup();

    if (signalFlag) {
        puts("TorLux clean exit, SIGINT");
    }
    else {
        puts("TorLux clean exit");
    }
}

void TorLux::initiate() {
    Generate::randBytes((char*) initcode, 32);

    puts("Send this token in a secure manner:\n");
    for (int i = 0; i < 32; i++) printf("%02x", initcode[i]);
    for (int i = 0; i < 56; i++) putchar(myAddr[i]);
    puts("\n\nWaiting for connection...");

    Server::waitForConnection();
    if (TorLux::exitFlag) return;
    Socks::makeRequest();
}

void TorLux::join() {
    Generate::randBytes((char*) chatcode, 32);

    puts("Joining...");

    std::vector<char> msg;
    for (int i = 0; i < 32; i++) msg.push_back(initcode[i]);
    for (int i = 0; i < 32; i++) msg.push_back(chatcode[i]);
    for (int i = 0; i < HOSTNAME_LEN; i++) msg.push_back(myAddr[i]);

    std::vector<char> resp;
    Socks::transmit(msg, resp);
}