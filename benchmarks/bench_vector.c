#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/vector.h"

#define DIM (int)1e7
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

void time_dot_product(Vector *u, Vector *v) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < REPEAT; ++i) {
        float _Complex result = vector_inner_product(u, v);
        (void)result; // suppress unused warning
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = get_elapsed_time(start, end);

    printf("[benchmark] dot product on dim=%d repeated %d times: %.6f sec (%.6f ms avg)\n",
           DIM, REPEAT, elapsed, (elapsed * 1000) / REPEAT);
}

int main() {
    Vector *u = rademacher_vector(DIM);
    Vector *v = rademacher_vector(DIM);

    if (!u || !v) {
        fprintf(stderr, "Vector allocation failed\n");
        return EXIT_FAILURE;
    }

    time_dot_product(u, v);

    free_vector(u); free_vector(v); free(u); free(v);
    return EXIT_SUCCESS;
}