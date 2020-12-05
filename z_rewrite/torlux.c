#include "torlux.h"

volatile atomic_flag exitFlag = ATOMIC_FLAG_INIT;

char initcode[32], chatcode[32];

pthread_mutex_t chatMutex = PTHREAD_MUTEX_INITIALIZER;
char *incoming = NULL;

pthread_t senderThread, serverThread;

char myAddr[56], targetAddr[56];