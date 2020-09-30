#ifndef INCLUDE_CONTEXT
#define INCLUDE_CONTEXT

#include <string>

#define HOSTNAME_LEN 56
#define HOSTNAME_LEN_ONION (56 + 6)
#define TOKEN_LEN (32 * 2 + 56)

class Context {
public:
    Context() = delete;
    ~Context() = delete;
    static std::string myAddr, targetAddr;
private:   

};

#endif /* INCLUDE_CONTEXT */
