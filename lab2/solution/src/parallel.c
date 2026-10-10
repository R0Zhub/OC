#include "common.h"
#include <pthread.h>

typedef struct {
    int thread_id;
    int num_threads;
    int rows;
    int cols;
    int is_erode;
    int iterations;
} ThreadArgs;

static double* g_current;
static double* g_next;
static pthread_barrier_t barrier;

static void* thread_func(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int rows_per_thread = args->rows / args->num_threads;
    int remainder = args->rows % args->num_threads;
    int start_row = args->thread_id * rows_per_thread + (args->thread_id < remainder ? args->thread_id : remainder);
    int end_row = start_row + rows_per_thread + (args->thread_id < remainder ? 1 : 0);

    for (int iter = 0; iter < args->iterations; iter++) {
        pthread_barrier_wait(&barrier); // Синхронизация начала итерации
        
        const double* curr = g_current;
        double* nxt = g_next;
        
        for (int i = start_row; i < end_row; i++) {
            for (int j = 0; j < args->cols; j++) {
                double val = curr[i * args->cols + j];
                for (int di = -1; di <= 1; di++) {
                    int ni = i + di;
                    if (ni < 0 || ni >= args->rows) continue;
                    int row_offset = ni * args->cols;
                    for (int dj = -1; dj <= 1; dj++) {
                        int nj = j + dj;
                        if (nj < 0 || nj >= args->cols) continue;
                        double neighbor = curr[row_offset + nj];
                        if (args->is_erode) {
                            if (neighbor < val) val = neighbor;
                        } else {
                            if (neighbor > val) val = neighbor;
                        }
                    }
                }
                nxt[i * args->cols + j] = val;
            }
        }
        pthread_barrier_wait(&barrier); // Синхронизация конца итерации
    }
    return NULL;
}

static void process_parallel(const double* in, double* out, int rows, int cols, int k, int num_threads, int is_erode) {
    if (k <= 0) { for (int i = 0; i < rows * cols; i++) out[i] = in[i]; return; }
    if (num_threads < 1) num_threads = 1;
    
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    ThreadArgs* args = (ThreadArgs*)malloc(num_threads * sizeof(ThreadArgs));
    double* current = (double*)malloc(rows * cols * sizeof(double));
    double* next = (double*)malloc(rows * cols * sizeof(double));
    
    for (int i = 0; i < rows * cols; i++) current[i] = in[i];
    pthread_barrier_init(&barrier, NULL, num_threads + 1); // +1 для главного потока
    
    for (int t = 0; t < num_threads; t++) {
        args[t].thread_id = t;
        args[t].num_threads = num_threads;
        args[t].rows = rows;
        args[t].cols = cols;
        args[t].is_erode = is_erode;
        args[t].iterations = k;
        pthread_create(&threads[t], NULL, thread_func, &args[t]);
    }
    
    for (int iter = 0; iter < k; iter++) {
        g_current = current;
        g_next = next;
        pthread_barrier_wait(&barrier); // Ждем, пока потоки прочитают указатели
        pthread_barrier_wait(&barrier); // Ждем, пока потоки завершат итерацию
        
        double* temp = current;
        current = next;
        next = temp;
    }
    
    for (int t = 0; t < num_threads; t++) pthread_join(threads[t], NULL);
    
    for (int i = 0; i < rows * cols; i++) out[i] = current[i];
    
    free(threads); free(args); free(current); free(next);
    pthread_barrier_destroy(&barrier);
}

void erode_parallel(const double* in, double* out, int rows, int cols, int k, int num_threads) {
    process_parallel(in, out, rows, cols, k, num_threads, 1);
}

void dilate_parallel(const double* in, double* out, int rows, int cols, int k, int num_threads) {
    process_parallel(in, out, rows, cols, k, num_threads, 0);
}