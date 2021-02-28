#include "server.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "torlux.h"
#include "tool.h"

static int sockfd = -1;

void server_init() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("Could not create listening socket");
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(42069);

    if (bind(sockfd, (struct sockaddr*) &server, sizeof(server))) {
        die("Listening socket bind failed");
    }

    if (listen(sockfd, 8)) {
        die("Server listen() function failed");
    }
}

void* server_run(void *args) {
    struct pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    char buf[256];
    const char *response = "whats never beens never been so\n";

    while (!atomic_flag_test_and_set(&exitFlag)) {
        atomic_flag_clear(&exitFlag);
        pfd.revents = 0;
        poll(&pfd, 1, 500);
        if (pfd.revents == POLLIN) {
            int tmpfd = accept(sockfd, NULL, NULL);
            ssize_t recved = recv(tmpfd, buf, 256, 0);
            
            if (recved > 32) {
                bool flag = true;
                for (int i = 0; i < 32; i++) 
                    if (buf[i] != chatcode[i]) { flag = false; break; }
                
                if (flag) { // if transmission is legit
                    int ptr = 0;
                    pthread_mutex_lock(&chatMutex);
                    for (int i = 32; i < recved; i++) incoming[ptr++] = buf[i];
                    incoming[ptr] = '\0';
                    pthread_mutex_unlock(&chatMutex);

                    read_discard(tmpfd);
                    write(tmpfd, response, strlen(response));
                }
            }

            close(tmpfd);
        }
    }
    return NULL;
}

void listen_for_connect() {
    struct pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    char buf[256];
    const char *response = "use superior free software like GAHNOO/Linux -> happy rms noises";

    bool good = false;

    while (!good && !atomic_flag_test_and_set(&exitFlag)) {
        atomic_flag_clear(&exitFlag);
        pfd.revents = 0;
        poll(&pfd, 1, 500);

        if (pfd.revents == POLLIN) {
            int tmpfd = accept(sockfd, NULL, NULL);

            ssize_t recved = recv(tmpfd, buf, 256, 0);
            if (recved == 64 + HOSTNAME_LEN) {
                good = true;

                for (int i = 0; i < 32; i++) {
                    if (buf[i] != initcode[i]) {
                        puts("Bad request: invalid initialization code");
                        good = false; break;
                    }
                }

                if (good) {
                    for (int i = 64; i < recved; i++) {
                        if (!validb32(buf[i])) {
                            good = false;
                            puts("Bad request: onion address is invalid base32");
                            break;
                        }
                        targetAddr[i - 64] = buf[i];
                    }

                    if (good) {
                        memcpy(targetAddr + HOSTNAME_LEN, ".onion", 6);
                        memcpy(chatcode, buf + 32, 32);
                    }
                }
            }
            else puts("Bad request: invalid size");

            if (good) break;
        }
    }
}

void server_cleanup() {
    if (sockfd != -1) {
        close(sockfd);
    }
}