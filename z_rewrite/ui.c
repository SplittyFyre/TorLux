#include <ncurses.h>

static bool inited = false;
static int sizey, sizex;
static WINDOW *chat, *input;
static char s[1024]; // large enough
static size_t slen = 0;
void parseCommand();

#define INPUT_HEIGHT 2

#define PEER_COLOUR 1
#define USER_COLOUR 2
#define PORTAL_COLOUR 3

void ui_init() {
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

bool ui_update() {

}

void ui_cleanup() {
    if (inited) {
        delwin(chat);
        delwin(input);
        endwin();
    }
}