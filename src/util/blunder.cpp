#include "blunder.h"

#include "Socks.h"
#include "Server.h"
#include "UI.h"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

void die(const char *fmt, ...) {
    Socks::cleanup();
    Server::cleanup();
    UI::cleanup();

    puts("Program died badly:");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    puts("");

    exit(1);
}