#include "server.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

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


}