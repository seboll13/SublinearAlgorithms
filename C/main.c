#include <time.h>
#include <stdarg.h>
#include "projections.h"
#include "matrix.h"
#include <pthread.h>

#define DIM 1e6


int main() {
    srand(time(NULL)); // Set a different seed for random generation
    
    printf("START OF PROGRAM...\n");
    
    Vector *u = rademacher_vector(DIM);
    Vector *v = rademacher_vector(DIM);
    
    // Time function execution
    clock_t start = clock();
    vector_dot_product(u, v);
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken for standard dot product: %.3f\n", time_taken);

    clock_t start2 = clock();
    optimised_vector_dot_product(u, v);
    clock_t end2 = clock();
    double time_taken2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("Time taken for optimised dot product: %.3f\n", time_taken2);

    clock_t start3 = clock();
    vector_dot_product_multithreaded(u, v, 2);
    clock_t end3 = clock();
    double time_taken3 = (double)(end3 - start3) / CLOCKS_PER_SEC;
    printf("Time taken for multithreaded dot product: %.3f\n", time_taken3);
    
    // Cleanup
    free(u);
    free(v);

    printf("END OF PROGRAM.\n");
}