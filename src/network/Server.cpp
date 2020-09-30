#include "Server.h"

#include "TorLux.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>

static int sockfd = -1;

void Server::init() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        puts("Could not create listening socket");
        exit(1);
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(42069);

    if (bind(sockfd, (sockaddr*) &server, sizeof(server))) {
        puts("Listening socket bind failed");
        exit(1);
    }

    if (listen(sockfd, 8)) {
        puts("Server listen(sockfd, 8) failed");
        exit(1);
    }
}

void* Server::run(void *) {
    pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    const int cs = sizeof(sockaddr_in);
    sockaddr_in client;

    char buf[256];
    const char *message = "HELLO_I_AM_HITCHCOCK\n";

    while (!TorLux::exitFlag) {
        pfd.revents = 0;
        poll(&pfd, 1, 500);
        if (pfd.revents == POLLIN) {
            int tmpfd = accept(sockfd, (sockaddr*) &client, (socklen_t*) &cs);
            
            ssize_t recved = recv(tmpfd, buf, 256, 0);

            std::string msg = "here: ";
            for (int i = 0; i < std::min(recved, ssize_t(10)); i++) {
                msg.push_back(buf[i]);
            }

            pthread_mutex_lock(&TorLux::chatMutex);
            TorLux::chatBuffer.push_back(msg);
            pthread_mutex_unlock(&TorLux::chatMutex);

            while (recved == 256) {
                recved = recv(tmpfd, buf, 256, 0);
            }

            write(tmpfd, message, strlen(message));

            close(tmpfd);
        }
    }

    return nullptr;
}

void Server::cleanup() {
    if (sockfd != -1) {
        close(sockfd);
    }
}