#ifndef INCLUDE_TORLUX
#define INCLUDE_TORLUX

#include <signal.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

#define HOSTNAME_LEN 56
#define HOSTNAME_LEN_ONION (56 + 6)
#define TOKEN_LEN (32 * 2 + 56)

extern volatile atomic_flag exitFlag;
extern volatile sig_atomic_t signalFlag;
extern char initcode[32], chatcode[32];

extern pthread_mutex_t chatMutex;
extern char incoming[256];
extern bool hasIncoming;

extern char myAddr[HOSTNAME_LEN_ONION], targetAddr[HOSTNAME_LEN_ONION];

void torlux_run(int mode, const char *token);

#endif /* INCLUDE_TORLUX */
