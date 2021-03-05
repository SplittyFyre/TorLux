#include "torlux.h"

#include "tool.h"
#include "server.h"
#include "sender.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile atomic_flag exitFlag = ATOMIC_FLAG_INIT;
volatile sig_atomic_t signalFlag = false;

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

void initiate() {
    csrng(initcode, 32);

    puts("Send this token in a secure manner:\n");
    for (int i = 0; i < 32; i++) printf("%02x", initcode[i]);
    for (int i = 0; i < 56; i++) putchar(myAddr[i]);
    puts("\n\nWaiting for connection...");

    server_listen_for_connect();
    if (atomic_flag_test_and_set(&exitFlag)) return;
    else atomic_flag_clear(&exitFlag);
}

void join() {
    csrng(chatcode, 32);

    puts("Joining...");
    
    char msg[128];
    memcpy(msg, initcode, 32);
    memcpy(msg + 32, chatcode, 32);
    memcpy(msg + 64, myAddr, HOSTNAME_LEN);
    transmit(msg, 64 + HOSTNAME_LEN);
}

void torlux_run(int mode, const char *token) {
    if (mode) {
        parseToken(token);
    }

    server_init();

    if (mode) join();
    else initiate();
    
    if (atomic_flag_test_and_set(&exitFlag)) {
        server_cleanup();
        puts("Received SIGINT, exiting");
    }
    else atomic_flag_clear(&exitFlag);

    ui_init();

    pthread_create(&serverThread, NULL, server_run, NULL);
    pthread_create(&senderThread, NULL, backgroundSender, NULL);

    while (true) {
        if (signalFlag) {
            atomic_flag_test_and_set(&exitFlag);
            break;
        }

        if (!ui_update()) {
            atomic_flag_test_and_set(&exitFlag);
            break;
        }
    }

    pthread_join(serverThread, NULL);
    senderPrepareJoin();
    pthread_join(senderThread, NULL);

    server_cleanup();
    ui_cleanup();    
}
