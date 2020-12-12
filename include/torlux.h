#ifndef INCLUDE_TORLUX
#define INCLUDE_TORLUX

#include <signal.h>
#include <stdatomic.h>
#include <pthread.h>

#define HOSTNAME_LEN 56
#define HOSTNAME_LEN_ONION (56 + 6)
#define TOKEN_LEN (32 * 2 + 56)

extern char myAddr[HOSTNAME_LEN_ONION], targetAddr[HOSTNAME_LEN_ONION];

#endif /* INCLUDE_TORLUX */