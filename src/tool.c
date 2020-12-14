#include "tool.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>


static char discardbuf[4096];
void read_discard(int sockfd) {
    while (recv(sockfd, discardbuf, 4096, 0) > 0);
}


int hcti(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

bool validb32(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '2' && c <= '7') || (c >= 'A' && c <= 'Z');
}

void die(const char *fmt, ...) {
    puts("Program died badly:");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf("\nerrno string: %s (%d)\n", strerror(errno), errno);

    exit(1);
}

void csrng(char *dest, size_t s) {
    FILE *fin = fopen("/dev/urandom", "rb");
    if (fin == NULL) {
        die("Failed to open /dev/urandom for csrng");
    }
    fread(dest, 1, s, fin);
    fclose(fin);
}


