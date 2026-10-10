#include "transform.h"

size_t upper_case(char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = (unsigned char)buf[i];
        if (c >= 'a' && c <= 'z') c = (unsigned char)(c - 32);
        buf[i] = (char)c;
    }
    return len;
}

size_t collapse_spaces(const char *in, size_t len, char *out, int *prev) {
    size_t m = 0;
    for (size_t i = 0; i < len; ++i) {
        char c = in[i];
        if (c == ' ' && *prev) continue;   
        out[m++] = c;
        *prev = (c == ' ');
    }
    return m;
}