#include "UI.h"

#include "Sender.h"
#include "TorLux.h"
#include <ncurses.h>
#include <string>

static bool inited = false;
static int sizey, sizex;
static WINDOW *chat, *input;
static std::string s;
void parseCommand();

#define INPUT_HEIGHT 2

#define PEER_COLOUR 1
#define USER_COLOUR 2
#define PORTAL_COLOUR 3

void UI::init() {
    initscr();

    use_default_colors();
    start_color();
    
    init_pair(PEER_COLOUR, COLOR_BLUE, -1);
    init_pair(USER_COLOUR, COLOR_RED, -1);
    init_pair(PORTAL_COLOUR, COLOR_GREEN, -1);

    getmaxyx(stdscr, sizey, sizex);
    chat = newwin(sizey - INPUT_HEIGHT, sizex, 0, 0);
    input = newwin(INPUT_HEIGHT, sizex, sizey - INPUT_HEIGHT, 0);
    refresh();
    
    noecho();
    timeout(1); // if negative, blocking, if 0, nonblocking, else, milliseconds
    scrollok(chat, true);

    wprintw(input, ">>> ");
    wrefresh(input);

    wattron(chat, COLOR_PAIR(PORTAL_COLOUR));
    wprintw(chat, "TorLux session initiated. You may now chat securely and anonymously.\n\n");
    wattroff(chat, COLOR_PAIR(PORTAL_COLOUR));
    wrefresh(chat);

    inited = true;
}

bool UI::update() {

    pthread_mutex_lock(&TorLux::chatMutex);
    for (auto &a : TorLux::chatBuffer) {
        wattron(chat, COLOR_PAIR(PEER_COLOUR));
        wprintw(chat, "Anon: ");
        wattroff(chat, COLOR_PAIR(PEER_COLOUR));
        wprintw(chat, "%s\n\n", a.c_str());
    }
    TorLux::chatBuffer.clear();
    pthread_mutex_unlock(&TorLux::chatMutex);

    int c = getch();
    if (c == ERR) {
        wrefresh(chat); // for ^
        return true;
    }

    if (c == KEY_BACKSPACE || c == KEY_DC || c == 127) {
        if (!s.empty()) s.pop_back();
    }
    else {
        if (c == '\n') {

            if (s == "/exit") {
                return false;
            }

            if (s.front() == '/') {
                parseCommand();
                s.clear();
            }

            if (!s.empty()) {
                pthread_mutex_lock(&Sender::mutex);
                Sender::toSend.push_back(s);
                pthread_cond_signal(&Sender::cond);
                pthread_mutex_unlock(&Sender::mutex);

                wattron(chat, COLOR_PAIR(USER_COLOUR));
                wprintw(chat, "You: ");
                wattroff(chat, COLOR_PAIR(USER_COLOUR));
                wprintw(chat, "%s\n\n", s.c_str());

                s.clear();
            }
        }
        else {
            s.push_back(c);
        }
    }

    wclear(input);
    wprintw(input, ">>> %s", s.c_str());

    wrefresh(chat);
    wrefresh(input);

    return true;
}

void UI::cleanup() {
    if (inited) {
        delwin(chat);
        delwin(input);
        endwin();
    }
}



void parseCommand() {

    

}