#include <cstdio>
#include <cstring>
#include <pwd.h>
#include <unistd.h>
#include <signal.h>

#include <fstream>
#include <string>
#include "TorLux.h"
#include "Generate.h"

void printhelp() {
    puts("Usage: torlux <mode> ...");
    puts("");
    puts("Modes:");
    puts("  help           show this help dialogue");
    puts("  generate       creates or refreshes data in ~/.torlux");
    puts("  initiate       initiates a torlux chat session and provides a token");
    puts("  join           joins a torlux chat session, pass token as next argument");
}

void ensureData() {
    passwd *pw = getpwuid(getuid());
    std::string file = pw->pw_dir;
    file += "/.torlux/hs/hostname";

    std::ifstream fin(file);
    if (fin.fail()) {
        puts("Invalid data to establish connection");
        puts("Try running torlux generate followed by tor with the provided ~/.torlux/torrc");
        puts("Important: tor needs to generate hostname files for torlux to work");
        exit(0);
    }
    
    TorLux::myAddr = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()); // sluuurp
    while (TorLux::myAddr.back() == '\n') TorLux::myAddr.pop_back();
    if (TorLux::myAddr.length() != HOSTNAME_LEN_ONION) {
        printf("Invalid onion hostname length: %d; should be %d\n", int(TorLux::myAddr.length()), HOSTNAME_LEN_ONION);
        exit(0);
    }
}

void handleSigint(int) {
    TorLux::signalFlag = true;
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
        Generate::generate();
        puts("Generation complete");
    }
    else if (strcmp(mode, "initiate") == 0) {
        ensureData();
        TorLux::run(false, nullptr);
    }
    else if (strcmp(mode, "join") == 0) {
        ensureData();
        if (argc < 3) {
            puts("Insufficient arguments, join needs token");
            return 0;
        }
        TorLux::run(true, argv[2]);
    }
    else {
        printf("Unknown mode: %s\n", mode);
    }
    
    return 0;
}