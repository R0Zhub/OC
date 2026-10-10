#include "common.h"
#include <unistd.h>
#include <getopt.h>

void print_usage(const char* prog) {
    printf("Usage: %s [options]\n", prog);
    printf("Options:\n");
    printf("  -r <rows>       Number of rows (default: 1000)\n");
    printf("  -c <cols>       Number of columns (default: 1000)\n");
    printf("  -k <iterations> Number of iterations K (default: 10)\n");
    printf("  -t <threads>    Max number of threads (default: 4)\n");
    printf("  -h              Show this help message\n");
}

int main(int argc, char* argv[]) {
    int rows = 1000, cols = 1000, k = 10, num_threads = 4;
    int opt;
    while ((opt = getopt(argc, argv, "r:c:k:t:h")) != -1) {
        switch (opt) {
            case 'r': rows = atoi(optarg); break;
            case 'c': cols = atoi(optarg); break;
            case 'k': k = atoi(optarg); break;
            case 't': num_threads = atoi(optarg); break;
            case 'h': print_usage(argv[0]); return 0;
            default: print_usage(argv[0]); return 1;
        }
    }
    
    if (rows <= 0 || cols <= 0 || k <= 0 || num_threads <= 0) {
        fprintf(stderr, "Invalid parameters. All values must be > 0.\n");
        return 1;
    }
    
    printf("Matrix: %dx%d, Iterations: %d, Threads: %d\n", rows, cols, k, num_threads);
    
    size_t size = rows * cols * sizeof(double);
    double* input = (double*)malloc(size);
    double* out_seq_erode = (double*)malloc(size);
    double* out_seq_dilate = (double*)malloc(size);
    double* out_par_erode = (double*)malloc(size);
    double* out_par_dilate = (double*)malloc(size);
    
    srand(42); // Фиксированный seed для воспроизводимости
    for (int i = 0; i < rows * cols; i++) input[i] = (double)rand() / RAND_MAX * 100.0;
    
    double start, end;
    
    start = get_time_ms(); erode_sequential(input, out_seq_erode, rows, cols, k); end = get_time_ms();
    double time_seq_erode = end - start;
    
    start = get_time_ms(); dilate_sequential(input, out_seq_dilate, rows, cols, k); end = get_time_ms();
    double time_seq_dilate = end - start;
    
    start = get_time_ms(); erode_parallel(input, out_par_erode, rows, cols, k, num_threads); end = get_time_ms();
    double time_par_erode = end - start;
    
    start = get_time_ms(); dilate_parallel(input, out_par_dilate, rows, cols, k, num_threads); end = get_time_ms();
    double time_par_dilate = end - start;
    
    printf("\n--- Results ---\n");
    printf("Sequential Erode time:   %.3f ms\n", time_seq_erode);
    printf("Parallel Erode time:     %.3f ms\n", time_par_erode);
    printf("Sequential Dilate time:  %.3f ms\n", time_seq_dilate);
    printf("Parallel Dilate time:    %.3f ms\n", time_par_dilate);
    
    // Верификация корректности
    int match = 1;
    for (int i = 0; i < rows * cols; i++) {
        if (fabs(out_seq_erode[i] - out_par_erode[i]) > 1e-9 || fabs(out_seq_dilate[i] - out_par_dilate[i]) > 1e-9) {
            match = 0; break;
        }
    }
    printf("\nVerification: Results match = %s\n", match ? "YES" : "NO");
    
    free(input); free(out_seq_erode); free(out_seq_dilate); free(out_par_erode); free(out_par_dilate);
    return 0;
}