#include "tool.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

void die(const char *fmt, ...) {
    puts("Program died badly:");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf("\nerrno string: %s (%d)\n", strerror(errno), errno);

    exit(1);
}