#include "TorLux.h"

#include "Socks.h"
#include "UI.h"
#include "Server.h"
#include "Context.h"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

std::atomic<bool> TorLux::exitFlag(false);

uint8_t TorLux::initcode[32], TorLux::chatcode[32];

pthread_mutex_t TorLux::chatMutex;
std::vector<std::string> TorLux::chatBuffer;

int hcti(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

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
        if (!((c >= 'a' && c <= 'z') || (c >= '2' && c <= '7') || (c >= 'A' && c <= 'Z'))) {
            puts("Invalid token");
            exit(0);
        }
        Context::targetAddr.push_back(c);
    }
    Context::targetAddr += ".onion";
}

void TorLux::run(bool mode, const char *token) {

    Socks::init();
    Server::init();
    UI::init();

    pthread_t serverThread;
    pthread_create(&serverThread, nullptr, Server::run, nullptr);

    while (true) {
        if (!UI::update()) {
            exitFlag = true;
            break;
        }
    }

    pthread_join(serverThread, NULL);

    Socks::cleanup();
    Server::cleanup();
    UI::cleanup();
    puts("eyo!");
}

void TorLux::initiate() {
    uint8_t token[TOKEN_LEN];
    
}

void TorLux::join() {

}