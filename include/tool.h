#ifndef INCLUDE_TOOL
#define INCLUDE_TOOL

#include <stdlib.h>
#include <stdbool.h>

void read_discard(int sockfd);

int hcti(char c);
bool validb32(char c);
void die(const char *fmt, ...);

void csrng(char *dest, size_t s);

void generateConfig();

#endif /* INCLUDE_TOOL */
