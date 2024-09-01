#include <time.h>
#include <stdarg.h>
#include "projections.h"
#include "matrix.h"
#include <pthread.h>

#define DIM 1e8


int main() {
    srand(time(NULL)); // Set a different seed for random generation
    
    printf("START OF PROGRAM...\n");
    
    clock_t init_start = clock();
    Vector *u = rademacher_vector(DIM);
    Vector *v = rademacher_vector(DIM);
    clock_t init_end = clock();
    printf("Vectors initialised in %.3f seconds.\n", (double)(init_end - init_start) / CLOCKS_PER_SEC);
    
    // Time function execution
    clock_t start = clock();
    vector_dot_product(u, v);
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken for standard dot product: %.3f\n", time_taken);

    clock_t start2 = clock();
    vector_dot_product_optimised(u, v);
    clock_t end2 = clock();
    double time_taken2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("Time taken for optimised dot product: %.3f\n", time_taken2);
    
    // Cleanup
    free(u);
    free(v);

    printf("END OF PROGRAM.\n");
}