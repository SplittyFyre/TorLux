#include "Socks.h"

#include "Context.h"

#include "blunder.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

static int sockfd = -1;

void Socks::init() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("Failed to create socket in Socks init");
    }
    
    sockaddr_in target;
    target.sin_addr.s_addr = inet_addr("127.0.0.1");
    target.sin_family = AF_INET;
    target.sin_port = htons(9350);

    if (connect(sockfd, (sockaddr*) &target, sizeof(target))) {
        die("Failed to connect to SOCKS5 proxy, is tor running on 9350?");
    }

    // initial identifying request
    const char iden[3] = { 0x05, 0x01, 0x00 };
    if (send(sockfd, iden, sizeof(iden), 0) == -1) {
        die("Failed to send initial request to SOCKS5 proxy");
    }

    char idenrecv[2];
    if (recv(sockfd, idenrecv, 2, 0) == -1) {
        die("Failed to receive initial response from SOCKS5 proxy");
    }
    if (idenrecv[1] != 0x00) {
        die("Failed to authenticate for SOCKS5 proxy");
    }

    char request[4 + 1 + 62 + 2];
    request[0] = 0x05; // SOCKS5
    request[1] = 0x01; // connect
    request[2] = 0x00; // reserved
    request[3] = 0x03; // to give domain name
    request[4] = 62; // domain length of onion v3 always 56 + 6 = 62
    for (int i = 0; i < 62; i++) request[5 + i] = Context::targetAddr[i];
    uint16_t port = htons(80);
    memcpy(request + 67, &port, 2);

    if (send(sockfd, request, sizeof(request), 0) == -1) {
        die("Failed to transmit SOCKS5 request");
    }
    
    char requestResp[10];
    if (recv(sockfd, requestResp, 10, 0) == -1) {
        die("Failed to receive SOCKS5 response to request");
    }
    if (requestResp[1] != 0x00) {
        die("SOCKS5 request failed with code 0x%x != 0x00\n", requestResp[1]);
    }
}

void Socks::transmit(const std::vector<char> &data, std::vector<char> &resp) {
    
    if (send(sockfd, data.data(), data.size(), 0) == -1) {
        die("Failed to transmit real data to SOCKS5");
    }

    size_t bufsize = 2048;
    resp.resize(2048);
    
    while (true) {
        ssize_t recved = recv(sockfd, resp.data() + bufsize - 2048, 2048, 0);
        if (recved < 2048) {
            if (recved == -1) {
                die("Error receiving response from SOCKS5 proxy");
            }
            resp.resize(bufsize - (2048 - recved));
            break;
        }
        bufsize += 2048;
        resp.resize(bufsize);
    }
}

void Socks::cleanup() {
    if (sockfd != -1) {
        close(sockfd);
    }
}