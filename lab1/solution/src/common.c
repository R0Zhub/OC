#include "common.h"

#include <stdio.h>     
#include <stdlib.h>   
#include <unistd.h>    

void die(const char *who, const char *msg) {
    char buf[256];
    int n = snprintf(buf, sizeof(buf), "%s: error: %s\n", who, msg);
    if (n > 0) {
        ssize_t w = write(STDERR_FILENO, buf, (size_t)n);
        (void)w;
    }
    exit(EXIT_FAILURE);
}

void write_all(int fd, const char *buf, size_t n) {
    while (n > 0) {
        ssize_t w = write(fd, buf, n);
        if (w < 0) die("write_all", "write");
        buf += w;
        n   -= (size_t)w;
    }
}

ssize_t read_line(const char *who, int fd, char *buf, size_t cap) {
    size_t i = 0;
    while (i + 1 < cap) {
        char c;
        ssize_t r = read(fd, &c, 1);
        if (r < 0) die(who, "read");
        if (r == 0) break;         
        buf[i++] = c;
        if (c == '\n') break;
    }
    if (i + 1 >= cap && (i == 0 || buf[i - 1] != '\n'))
        die(who, "input line too long");
    buf[i] = '\0';
    return (ssize_t)i;
}