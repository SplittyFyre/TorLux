#ifndef INCLUDE_SOCKS
#define INCLUDE_SOCKS

#include <vector>

namespace Socks {
    void init();
    void makeRequest();
    void cleanup();
    void transmit(const std::vector<char> &data, std::vector<char> &resp);
};

#endif /* INCLUDE_SOCKS */
