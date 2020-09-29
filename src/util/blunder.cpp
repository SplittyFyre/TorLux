#include "blunder.h"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

void die(const char *fmt, ...) {



    puts("Program died badly:");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    exit(1);
}