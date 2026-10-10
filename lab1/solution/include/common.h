#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>      
#include <sys/types.h>   

#define LINE_CAP 8192   
#define IO_CHUNK 4096    


void die(const char *who, const char *msg);


void write_all(int fd, const char *buf, size_t n);


ssize_t read_line(const char *who, int fd, char *buf, size_t cap);

#endif /* COMMON_H */