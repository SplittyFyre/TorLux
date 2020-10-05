#include "Server.h"

#include "TorLux.h"
#include "Context.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include "util.h"
#include <cstring>
#include <vector>

static int sockfd = -1;

void Server::init() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("Could not create listening socket");
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(42069);

    if (bind(sockfd, (sockaddr*) &server, sizeof(server))) {
        die("Listening socket bind failed");
    }

    if (listen(sockfd, 8)) {
        die("Server listen() function failed");
    }
}

void* Server::run(void *) {
    pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    const int cs = sizeof(sockaddr_in);
    sockaddr_in client;

    char buf[256];
    const char *message = "MESSAGE_ACKNOWLEDGED";

    while (!TorLux::exitFlag) {
        pfd.revents = 0;
        poll(&pfd, 1, 500);
        if (pfd.revents == POLLIN) {
            int tmpfd = accept(sockfd, (sockaddr*) &client, (socklen_t*) &cs);
            
            ssize_t recved = recv(tmpfd, buf, 256, 0);

            if (recved > 32) {
                bool flag = true;
                for (int i = 0; i < 32; i++) {
                    if (uint8_t(buf[i]) != TorLux::chatcode[i]) {
                        flag = false; break;
                    }
                }

                if (flag) {
                    std::string msg;
                    for (int i = 32; i < recved; i++) {
                        msg.push_back(buf[i]);
                    }
                    pthread_mutex_lock(&TorLux::chatMutex);
                    TorLux::chatBuffer.push_back(msg);
                    pthread_mutex_unlock(&TorLux::chatMutex);
                }
            }

            while (recved == 256) {
                recved = recv(tmpfd, buf, 256, 0);
            }

            write(tmpfd, message, strlen(message));

            close(tmpfd);
        }
    }

    return nullptr;
}

void Server::waitForConnection() {
    pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    const int cs = sizeof(sockaddr_in);
    sockaddr_in client;

    char buf[256];
    const char *message = "READYTOCHAT";

    bool good = false;

    while (!TorLux::exitFlag) {
        pfd.revents = 0;
        poll(&pfd, 1, 500);
        if (pfd.revents == POLLIN) {
            int tmpfd = accept(sockfd, (sockaddr*) &client, (socklen_t*) &cs);
            
            ssize_t recved = recv(tmpfd, buf, 256, 0);

            if (recved == 32 + 32 + HOSTNAME_LEN) {
                good = true;

                for (int i = 0; i < 32; i++) {
                    if (uint8_t(buf[i]) != TorLux::initcode[i]) {
                        puts("Received request with invalid initialization code");
                        good = false; break;
                    }
                }

                if (good) {
                    for (int i = 64; i < recved; i++) {
                        if (!validb32(buf[i])) {
                            good = false;
                            Context::targetAddr.clear();
                            puts("Malformed request: invalid onion address");
                            break;
                        }
                        Context::targetAddr.push_back(buf[i]);
                    }

                    if (good) {
                        Context::targetAddr += ".onion";
                        for (int i = 32; i < 64; i++) {
                            TorLux::chatcode[i - 32] = uint8_t(buf[i]);
                        }
                    }
                }
            }
            else puts("Malformed request: invalid size");

            while (recved == 256) {
                recved = recv(tmpfd, buf, 256, 0);
            }

            write(tmpfd, message, strlen(message));

            close(tmpfd);

            if (good) break;
        }
    }
}

void Server::cleanup() {
    if (sockfd != -1) {
        close(sockfd);
    }
}