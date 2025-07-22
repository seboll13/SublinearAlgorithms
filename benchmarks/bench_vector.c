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

/**
 * @brief Create a dummy integer vector object
 * 
 * @param dim 
 * @param fill_value
 * @return Vector* 
 */
Vector *create_dummy_integer_vector(int dim, int fill_value) {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", dim);
    for (int i = 0; i < 3; i++)
        update_vector(v, fill_value, i);
    return v;
}

void time_dot_product(Vector *u, Vector *v) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < REPEAT; ++i) {
        float _Complex result = vector_dot_product(u, v);
        (void)result; // suppress unused warning
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = get_elapsed_time(start, end);

    printf("[benchmark] dot product on dim=%d repeated %d times: %.6f sec (%.6f ms avg)\n",
           DIM, REPEAT, elapsed, (elapsed * 1000) / REPEAT);
}

int main() {
    Vector *u = create_dummy_integer_vector(DIM, 1);
    Vector *v = create_dummy_integer_vector(DIM, 1);

    if (!u || !v) {
        fprintf(stderr, "Vector allocation failed\n");
        return EXIT_FAILURE;
    }

    time_dot_product(u, v);

    free_vector(u); free_vector(v); free(u); free(v);
    return EXIT_SUCCESS;
}