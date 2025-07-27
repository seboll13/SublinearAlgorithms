#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

#define DIM 1e8

double get_total_time(clock_t start, clock_t end) {
    return (double) (end - start) / CLOCKS_PER_SEC;
}

int main() {
    printf("START OF PROGRAM\n");

    clock_t start_init = clock();
    Vector *u = rademacher_vector(DIM);
    Vector *v = rademacher_vector(DIM);
    clock_t end_init = clock();
    printf("Vectors initialised in %.3f seconds\n", get_total_time(start_init, end_init));

    clock_t start = clock();
    vector_inner_product(u, v);
    clock_t end = clock();
    printf("Inner product computed in %.3f seconds\n", get_total_time(start, end));

    free_vector(u);
    free_vector(v);
    free(u);
    free(v);

    printf("END OF PROGRAM\n");
    return 0;
}