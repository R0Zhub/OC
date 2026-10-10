#include "common.h"

static void erode_once(const double* in, double* out, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double min_val = in[i * cols + j];
            for (int di = -1; di <= 1; di++) {
                int ni = i + di;
                if (ni < 0 || ni >= rows) continue;
                int row_offset = ni * cols;
                for (int dj = -1; dj <= 1; dj++) {
                    int nj = j + dj;
                    if (nj < 0 || nj >= cols) continue;
                    double neighbor = in[row_offset + nj];
                    if (neighbor < min_val) min_val = neighbor;
                }
            }
            out[i * cols + j] = min_val;
        }
    }
}

static void dilate_once(const double* in, double* out, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double max_val = in[i * cols + j];
            for (int di = -1; di <= 1; di++) {
                int ni = i + di;
                if (ni < 0 || ni >= rows) continue;
                int row_offset = ni * cols;
                for (int dj = -1; dj <= 1; dj++) {
                    int nj = j + dj;
                    if (nj < 0 || nj >= cols) continue;
                    double neighbor = in[row_offset + nj];
                    if (neighbor > max_val) max_val = neighbor;
                }
            }
            out[i * cols + j] = max_val;
        }
    }
}

void erode_sequential(const double* in, double* out, int rows, int cols, int k) {
    if (k <= 0) { for (int i = 0; i < rows * cols; i++) out[i] = in[i]; return; }
    double* current = (double*)malloc(rows * cols * sizeof(double));
    double* next = (double*)malloc(rows * cols * sizeof(double));
    for (int i = 0; i < rows * cols; i++) current[i] = in[i];
    
    for (int iter = 0; iter < k; iter++) {
        erode_once(current, next, rows, cols);
        double* temp = current; current = next; next = temp;
    }
    for (int i = 0; i < rows * cols; i++) out[i] = current[i];
    free(current); free(next);
}

void dilate_sequential(const double* in, double* out, int rows, int cols, int k) {
    if (k <= 0) { for (int i = 0; i < rows * cols; i++) out[i] = in[i]; return; }
    double* current = (double*)malloc(rows * cols * sizeof(double));
    double* next = (double*)malloc(rows * cols * sizeof(double));
    for (int i = 0; i < rows * cols; i++) current[i] = in[i];
    
    for (int iter = 0; iter < k; iter++) {
        dilate_once(current, next, rows, cols);
        double* temp = current; current = next; next = temp;
    }
    for (int i = 0; i < rows * cols; i++) out[i] = current[i];
    free(current); free(next);
}