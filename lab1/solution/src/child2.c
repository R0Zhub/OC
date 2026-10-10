#include "common.h"
#include "transform.h"

#include <unistd.h>
#include <stdlib.h>

int main(void) {
    char in[IO_CHUNK], out[IO_CHUNK];
    int prev = 0;                          
    for (;;) {
        ssize_t n = read(STDIN_FILENO, in, sizeof(in));
        if (n < 0) die("child2", "read");
        if (n == 0) break;
        size_t m = collapse_spaces(in, (size_t)n, out, &prev);
        write_all(STDOUT_FILENO, out, m);
    }
    return EXIT_SUCCESS;
}