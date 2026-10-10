#include "common.h"

#include <stdio.h>     
#include <stdlib.h>    
#include <unistd.h>    
#include <sys/wait.h>  


static void get_prog_dir(char *out, size_t cap) {
    char path[1024];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);

   
    if (len <= 0 || (size_t)len >= sizeof(path)) {
        const char msg[] = "parent: error: readlink\n";
        ssize_t w = write(STDERR_FILENO, msg, sizeof(msg) - 1);
        (void)w;
        exit(EXIT_FAILURE);
    }
    path[len] = '\0';

    ssize_t slash = -1;
    for (ssize_t i = len - 1; i >= 0; --i)
        if (path[i] == '/') { slash = i; break; }
    if (slash < 0) die("parent", "bad exe path");

    size_t dlen = (size_t)slash + 1;          
    if (dlen + 1 >= cap) die("parent", "dir path too long");
    for (size_t i = 0; i < dlen; ++i) out[i] = path[i];
    out[dlen] = '\0';
}


static void spawn(const char *dir, const char *name,
                  int in_fd, int out_fd,
                  int p1[2], int p2[2], int p3[2]) {
    pid_t pid = fork();
    if (pid < 0) die("parent", "fork");
    if (pid == 0) {
        if (dup2(in_fd,  STDIN_FILENO)  < 0) die("child-boot", "dup2 in");
        if (dup2(out_fd, STDOUT_FILENO) < 0) die("child-boot", "dup2 out");
        
        close(p1[0]); close(p1[1]);
        close(p2[0]); close(p2[1]);
        close(p3[0]); close(p3[1]);

        char exe[1024];
        int n = snprintf(exe, sizeof(exe), "%s%s", dir, name);
        if (n < 0 || (size_t)n >= sizeof(exe)) die("child-boot", "exe path too long");
        char *const args[] = { (char *)name, NULL };
        execv(exe, args);
        die("child-boot", "execv");        
    }
}

int main(void) {
    int p1[2], p2[2], p3[2];
    if (pipe(p1) < 0) die("parent", "pipe p1");   
    if (pipe(p2) < 0) die("parent", "pipe p2");   
    if (pipe(p3) < 0) die("parent", "pipe p3");   

    char dir[1024];
    get_prog_dir(dir, sizeof(dir));

    spawn(dir, "child1", p1[0], p2[1], p1, p2, p3);  
    spawn(dir, "child2", p2[0], p3[1], p1, p2, p3);  

  
    close(p1[0]); close(p2[0]); close(p2[1]); close(p3[1]);

    
    char inbuf[LINE_CAP], outbuf[LINE_CAP];
    for (;;) {
        ssize_t n = read_line("parent", STDIN_FILENO, inbuf, sizeof(inbuf));
        if (n == 0) break;                        
        if (n == 1 && inbuf[0] == '\n') break;    
        write_all(p1[1], inbuf, (size_t)n);

        ssize_t m = read_line("parent", p3[0], outbuf, sizeof(outbuf));
        if (m == 0) break;
        write_all(STDOUT_FILENO, outbuf, (size_t)m);
    }

    close(p1[1]);   

    
    for (;;) {
        ssize_t m = read_line("parent", p3[0], outbuf, sizeof(outbuf));
        if (m <= 0) break;
        write_all(STDOUT_FILENO, outbuf, (size_t)m);
    }
    close(p3[0]);

    int st;
    waitpid(-1, &st, 0);
    waitpid(-1, &st, 0);
    return EXIT_SUCCESS;
}