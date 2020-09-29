#include <cstdio>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>

#include <string>
#include <vector>
#include <iostream>
#include "Socks.h"
#include "Generate.h"
#include "UI.h"

void printhelp() {
    puts("Usage: torlux <mode> ...");
    puts("Modes:");
    puts("help     -> show this help dialogue");
    puts("generate -> creates or refreshes data in ~/.torlux");
    puts("initiate -> initiates a torlux chat session and provides a token");
    puts("join     -> joins a torlux chat session, pass token as next argument");
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
        puts("initiating...");
    }
    else if (strcmp(mode, "join") == 0) {
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

    Socks::init();

    std::string msg = "GET / HTTP/1.1\n\n";
    std::vector<char> data;
    for (char c : msg) data.push_back(c); 

    std::vector<char> resp;

    Socks::transmit(data, resp);

    for (char c : resp) std::cout << c;
    std::cout << '\n';

    Socks::cleanup();

    return 0;
}*/

/*static int sizey, sizex;

int main(void) {

    initscr();

    getmaxyx(stdscr, sizey, sizex);

    WINDOW *win = newwin(sizey - 2, sizex, 0, 0);
    WINDOW *input = newwin(2, sizex, sizey - 2, 0);
    refresh();

    std::string s;
    
    wprintw(input, ">>> ");
    wrefresh(input);
    noecho();
    timeout(1); // if negative, blocking, if 0, nonblocking, else, milliseconds
    scrollok(win, true);

    while (true) {
        int c = getch();
        if (c == ERR) continue;

        if (c == KEY_BACKSPACE || c == KEY_DC || c == 127) {
            if (!s.empty()) s.pop_back();
        }
        else {
            if (c == '\n') {

                if (s == "/exit") {
                    break;
                }

                if (!s.empty()) {
                    wprintw(win, "%s\n\n", s.c_str());
                    s.clear();
                }
            }
            else {
                s.push_back(c);
            }
        }

        wclear(input);
        wprintw(input, ">>> %s", s.c_str());

        wrefresh(win);
        wrefresh(input);
    }

    delwin(win);
    delwin(input);

    endwin();

    return 0;
}*/

/*static const uint16_t listenport = 42069;

int main(int argc, char **argv) {

    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Could not create socket");
        return 0;
    }

    sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("172.217.13.163");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    if (connect(socket_desc, (sockaddr*) &server, sizeof(server)) < 0) {
        puts("Connect error");
        return 0;
    }

    puts("Connection Complete");

    const char *message = "GET / HTTP/1.1\n\n";

    if (send(socket_desc, message, strlen(message), 0) < 0) {
        puts("Send failed");
        return 0;
    }

    puts("Data sent");

    char reply[2000];

    if (recv(socket_desc, reply, 2000, 0) < 0) {
        puts("recv failed");
    }

    puts("Received Reply:");
    puts(reply);

    close(socket_desc);

    return 0;
}*/

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

    fcntl(socket_desc, F_SETFL, O_NONBLOCK);

    while ((new_socket = accept(socket_desc, (sockaddr*) &client, (socklen_t*) &c))) {
        if (new_socket == -1) {
            continue;
        }
        recv(new_socket, data, 256, 0);
        printf("Connection accepted with: %s\n", data);
        write(new_socket, message, strlen(message));
    }

    return 0;
}*/