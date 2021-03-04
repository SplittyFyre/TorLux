#ifndef INCLUDE_TOOL
#define INCLUDE_TOOL

#include <stdbool.h>

int hcti(char c);
bool validb32(char c);
void die(const char *fmt, ...);

void csrng(char *dest, size_t s);

void generateConfig();

#endif /* INCLUDE_TOOL */
