#include "torlux.h"

#include "tool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile atomic_flag exitFlag = ATOMIC_FLAG_INIT;

char initcode[32], chatcode[32];

pthread_mutex_t chatMutex = PTHREAD_MUTEX_INITIALIZER;
char incoming[256];

pthread_t senderThread, serverThread;

char myAddr[HOSTNAME_LEN_ONION], targetAddr[HOSTNAME_LEN_ONION];

void parseToken(const char *token) {
    size_t len = strlen(token);
    if (len != TOKEN_LEN) {
        puts("Invalid token");
        exit(0);
        for (int i = 0; i < 32; i++) {
            int v1 = hcti(token[i * 2]), v2 = hcti(token[i * 2 + 1]);
            if (v1 == -1 || v2 == -1) {
                puts("Invalid token");
                exit(0);
            }
            initcode[i] = v1 * 16 + v2;
        }
    }
    for (unsigned i = 64; i < len; i++) {
        if (!validb32(token[i])) {
            puts("Invalid token");
            exit(0);
        }
        targetAddr[i - 64] = token[i];
    }
    memcpy(targetAddr + HOSTNAME_LEN, ".onion", 6);
}
