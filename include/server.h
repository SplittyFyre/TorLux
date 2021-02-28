#ifndef INCLUDE_SERVER
#define INCLUDE_SERVER

void server_init();
void* server_run(void *args);
void listen_for_connect();
void server_cleanup();

#endif /* INCLUDE_SERVER */
