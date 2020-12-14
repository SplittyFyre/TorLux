#include "server.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>

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

    const int cs = sizeof(struct sockaddr_in);
    struct sockaddr_in client;

    char buf[256];
    const char *reply = "whats never beens never been so\n";

    while (!atomic_flag_test_and_set(&exitFlag)) {
        atomic_flag_clear(&exitFlag);
        pfd.revents = 0;
        poll(&pfd, 1, 500);
        if (pfd.revents == POLLIN) {
            int tmpfd = accept(sockfd, (struct sockaddr*) &client, (socklen_t*) &cs);
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
                    write(tmpfd, reply, strlen(reply));
                }
            }
            
            close(tmpfd);
        }
    }
}