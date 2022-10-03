#include <time.h>
#include "projections.h"
#include "matrix.h"

int main() {
    printf("START OF PROGRAM...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for random generation
    
    // VECTOR PART
    Vector *v = rademacher_vector(3);
    print_vector(v);
    free_vector(v);
    free(v);

    // MATRIX PART
    Matrix *m = rademacher_matrix(3,3);
    print_matrix(m);
    free_matrix(m);
    free(m);

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("END OF PROGRAM.\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}