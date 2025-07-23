#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/matrix.h"

#define DIM (int)1e4
#define REPEAT 100

/**
 * @brief Get the elapsed time object
 * 
 * @param start 
 * @param end 
 * @return double 
 */
static inline double get_elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void time_matrix_trace(Matrix *m) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < REPEAT; ++i) {
        float _Complex trace = matrix_trace(m);
        (void)trace; // suppress unused warning
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = get_elapsed_time(start, end);

    printf("[benchmark] matrix trace on dim=%d repeated %d times: %.6f sec (%.6f ms avg)\n",
           m->rows, REPEAT, elapsed, (elapsed * 1000) / REPEAT);
}

int main() {
    Matrix *m = rademacher_matrix(DIM, DIM);

    if (!m) {
        fprintf(stderr, "Matrix allocation failed\n");
        return EXIT_FAILURE;
    }

    time_matrix_trace(m);

    free_matrix(m);
    free(m);
    return EXIT_SUCCESS;
}