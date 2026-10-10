#include "common.h"
#include "transform.h"

#include <unistd.h>    
#include <stdlib.h>   

int main(void) {
    char buf[IO_CHUNK];
    for (;;) {
        ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n < 0) die("child1", "read");
        if (n == 0) break;                
        size_t len = upper_case(buf, (size_t)n);
        write_all(STDOUT_FILENO, buf, len);
    }
    return EXIT_SUCCESS;
}   