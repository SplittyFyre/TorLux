#ifndef INCLUDE_SERVER
#define INCLUDE_SERVER

namespace Server {
    void init();
    void* run(void *);
    void waitForConnection();
    void cleanup();
} 


#endif /* INCLUDE_SERVER */
