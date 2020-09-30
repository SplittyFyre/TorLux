#include "blunder.h"

#include "Socks.h"
#include "UI.h"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

void die(const char *fmt, ...) {
    Socks::cleanup();
    UI::cleanup();

    puts("Program died badly:");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    exit(1);
}