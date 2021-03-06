#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "torlux.h"
#include "tool.h"

void socks5init(int sockfd) {
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
    memcpy(request + 5, targetAddr, 62);
    uint16_t port = htons(80);
    memcpy(request + 67, &port, 2);
    
    if (send(sockfd, request, sizeof(request), 0) == -1) {
        die("Failed to transmit SOCKS5 request");
    }

    char requestResp[16];
    if (recv(sockfd, requestResp, 16, 0) == -1) {
        die("Failed to receive SOCKS5 response to request");
    }
    if (requestResp[1] != 0x00) {
        die("SOCKS5 request failed with code 0x%x != 0x00\n", requestResp[1]);
    }
}

void transmit(char *data, size_t size) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        die("Failed to create socket in Socks init");
    }

    struct sockaddr_in target;
    target.sin_addr.s_addr = inet_addr("127.0.0.1");
    target.sin_family = AF_INET;
    target.sin_port = htons(9350);

    if (connect(sockfd, (struct sockaddr*) &target, sizeof(target))) {
        die("Failed to connect to SOCKS5 proxy, is tor running on 9350?");
    }

    socks5init(sockfd);

    if (send(sockfd, data, size, 0) == -1) {
        die("Failed to transmit 'real' data to SOCKS5");
    }

    read_discard(sockfd); // trash data
    close(sockfd); // very important lol
}





static char tosend[1024];
static size_t datalen;
static pthread_cond_t cond;
static pthread_mutex_t mutex;

void senderEnqueueData(char *s, size_t len) {
    datalen = len + 32;
    memcpy(tosend, chatcode, 32);
    pthread_mutex_lock(&mutex);
    memcpy(tosend + 32, s, len);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}
#include <stdio.h>
void* backgroundSender(void *args) {
    
    pthread_mutex_lock(&mutex);

    while (!atomic_flag_test_and_set(&exitFlag)) {
        atomic_flag_clear(&exitFlag);
        pthread_cond_wait(&cond, &mutex);
        if (atomic_flag_test_and_set(&exitFlag)) break;
        atomic_flag_clear(&exitFlag);
        transmit(tosend, datalen);
        puts("sendth");
    }

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void senderPrepareJoin() {
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}
