#include "UI.h"

#include "TorLux.h"
#include <ncurses.h>
#include <string>

static bool inited = false;
static int sizey, sizex;
static WINDOW *chat, *input;
static std::string s;

#define INPUT_HEIGHT 2

void UI::init() {
    initscr();
    getmaxyx(stdscr, sizey, sizex);
    chat = newwin(sizey - INPUT_HEIGHT, sizex, 0, 0);
    input = newwin(INPUT_HEIGHT, sizex, sizey - INPUT_HEIGHT, 0);
    refresh();

    noecho();
    timeout(1); // if negative, blocking, if 0, nonblocking, else, milliseconds
    scrollok(chat, true);

    inited = true;
}

bool UI::update() {

    pthread_mutex_lock(&TorLux::chatMutex);
    for (auto &a : TorLux::chatBuffer) {
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

            if (!s.empty()) {
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