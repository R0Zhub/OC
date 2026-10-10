#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

static inline double get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

void erode_sequential(const double* in, double* out, int rows, int cols, int k);
void dilate_sequential(const double* in, double* out, int rows, int cols, int k);
void erode_parallel(const double* in, double* out, int rows, int cols, int k, int num_threads);
void dilate_parallel(const double* in, double* out, int rows, int cols, int k, int num_threads);

#endif