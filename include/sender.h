#ifndef INCLUDE_SENDER
#define INCLUDE_SENDER

#include <pthread.h>

void transmit(char *data, size_t size);

void senderEnqueueData(char *s, size_t len);
void* backgroundSender(void*);
void senderPrepareJoin();

#endif /* INCLUDE_SENDER */