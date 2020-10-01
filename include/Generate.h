#ifndef INCLUDE_GENERATE
#define INCLUDE_GENERATE

#include <cstddef>

namespace Generate {
    void generate();

    void initRandom();
    void randBytes(char *dest, size_t n);
    void cleanupRandom();
};

#endif /* INCLUDE_GENERATE */
