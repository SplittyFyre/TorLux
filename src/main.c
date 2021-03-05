#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>

#include "tool.h"
#include "torlux.h"

void printhelp() {
    puts("Usage: torlux <mode> ...\n");
    puts("Modes:");
    puts("  help           show this help dialogue");
    puts("  generate       creates or refreshes data in ~/.torlux");
    puts("  initiate       initiates a torlux chat session and provides a token");
    puts("  join           joins a torlux chat session, pass token as next argument");
}

void ensureData() {
    struct passwd *pw = getpwuid(getuid());
    char buf[64];
    strcpy(buf, pw->pw_dir); strcat(buf, "/.torlux/hs/hostname");

    FILE *fin = fopen(buf, "r");
    if (fin == NULL) {
        puts("Invalid data to establish connection");
        puts("Try running torlux generate followed by tor with the provided ~/.torlux/torrc");
        puts("Important: tor needs to generate hostname files for torlux to work");
        exit(0);
    }

    fgets(buf, HOSTNAME_LEN_ONION + 1, fin);
    fclose(fin);

    if (strlen(buf) != HOSTNAME_LEN_ONION) {
        printf("Invalid onion hostname length: %zu; should be %d\n", strlen(buf), HOSTNAME_LEN_ONION);
        exit(0);
    }

    puts(buf);
}

void handleSigint(int i) {
    signalFlag = true;
}

int main(int argc, char **argv) {

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, handleSigint);

    if (argc < 2) {
        printhelp();
        return 0;
    }

    const char *mode = argv[1];

    if (strcmp(mode, "help") == 0) {
        printhelp();
        return 0;
    }
    else if (strcmp(mode, "generate") == 0) {
        generateConfig();
        puts("Generation complete");
    }
    else if (strcmp(mode, "initiate") == 0) {
        ensureData();
        torlux_run(0, NULL);
    }
    else if (strcmp(mode, "join") == 0) {
        ensureData();
        if (argc < 3) {
            puts("Insufficient arguments, join needs token");
            return 0;
        }
        torlux_run(1, argv[2]);
    }
    else {
        printf("Unknown mode: %s\n", mode);
    }
    
    return 0;
}