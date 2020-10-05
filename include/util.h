#ifndef INCLUDE_BLUNDER
#define INCLUDE_BLUNDER

inline int hcti(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

inline bool validb32(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '2' && c <= '7') || (c >= 'A' && c <= 'Z');
}

void die(const char *fmt, ...);

#endif /* INCLUDE_BLUNDER */
