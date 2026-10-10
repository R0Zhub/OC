#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <stddef.h>


size_t upper_case(char *buf, size_t len);


size_t collapse_spaces(const char *in, size_t len, char *out, int *prev);

#endif 