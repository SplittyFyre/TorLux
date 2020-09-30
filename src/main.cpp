#include <cstdio>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <pwd.h>
#include <poll.h>

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "Socks.h"
#include "TorLux.h"
#include "Generate.h"
#include "Context.h"
#include "UI.h"

void printhelp() {
    puts("Usage: torlux <mode> ...");
    puts("Modes:");
    puts("help     -> show this help dialogue");
    puts("generate -> creates or refreshes data in ~/.torlux");
    puts("initiate -> initiates a torlux chat session and provides a token");
    puts("join     -> joins a torlux chat session, pass token as next argument");
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

    Context::myAddr = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()); // sluuurp
    if (Context::myAddr.back() == '\n') Context::myAddr.pop_back();
    if (Context::myAddr.length() != HOSTNAME_LEN_ONION) {
        printf("Invalid onion hostname length: %d; should be %d\n", int(Context::myAddr.length()), HOSTNAME_LEN_ONION);
        exit(0);
    }
}

int main(int argc, char **argv) {

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
        puts("joining...");
    }
    else {
        printf("Unknown mode: %s\n", mode);
    }
    
    return 0;
}

/*int main(void) {

    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Could not create socket");
        return 0;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(socket_desc, (sockaddr*) &server, sizeof(server))) {
        puts("Bind failed");
        return 0;
    }

    puts("Bind done"); 

    listen(socket_desc, 3);

    const char *message = "<html>HELLO. I AM HITCHCOCK</html>\n\n";
    puts("Waiting for connections...");
    
    int c = sizeof(sockaddr_in);
    int new_socket;
    sockaddr_in client;

    char data[256];

    while (true) {
        pollfd pfd;
        pfd.fd = socket_desc;
        pfd.events = POLLIN;
        pfd.revents = 0;

        poll(&pfd, 1, 1000);
        puts("Poll done");
        if (pfd.revents == POLLIN) {
            new_socket = accept(socket_desc, (sockaddr*) &client, (socklen_t*) &c);
            if (new_socket == -1) {
                puts("FUCK");
                break;
            }
            recv(new_socket, data, 256, 0);
            printf("Connection accepted with: %s\n", data);
            write(new_socket, message, strlen(message));
        }
        
    }

    return 0;
}*/