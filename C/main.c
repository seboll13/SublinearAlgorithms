#include <time.h>
#include "projections.h"
#include "matrix.h"

#define DIM 1024

int main() {
    srand(time(NULL)); // Set a different seed for random generation
    
    printf("START OF PROGRAM...\n");

    // MATRIX PART
    Matrix *A = rademacher_matrix(DIM, DIM);
    Matrix *B = rademacher_matrix(DIM, DIM);
    
    clock_t start = clock();
    Matrix *C = matrix_mult(A, B);
    clock_t end = clock();
    
    free_matrix(C);
    free(C);
    
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC;
    printf("Execution time for standard matrix multiplication : %.3f [s]\n", execution_time);

    clock_t start2 = clock();
    Matrix *D = fast_matrix_mult(A, B);
    clock_t end2 = clock();

    free_matrix(D);
    free(D);

    double execution_time2 = (double)(end2-start2) / CLOCKS_PER_SEC;
    printf("Execution time for fast matrix multiplication : %.3f [s]\n", execution_time2);

    free_matrix(A);
    free_matrix(B);
    free(A);
    free(B);

    printf("END OF PROGRAM.\n");
}