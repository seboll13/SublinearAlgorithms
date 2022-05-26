#include <time.h>
#include "projections.h"
#include "matrix.h"

int main() {
    printf("START OF PROGRAM...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for random generation
    
    // VECTOR PART
    // Vector *u = malloc(sizeof(Vector));
    // init_vector(u, "U", 3);
    // for (int i = 0; i < u->capacity; i++)
    //     update_vector(u, 1.0f, i);
    // print_vector(u);
    // free_vector(u);
    // free(u);

    // MATRIX PART
    Matrix *m1 = malloc(sizeof(Matrix));
    Matrix *m2 = malloc(sizeof(Matrix));
    init_matrix(m1, "M1", 2, 2);
    init_matrix(m2, "M2", 3, 2);
    update_matrix(m1, 1.0f, 0, 0);
    update_matrix(m1, 0.0f, 1, 0);
    update_matrix(m1, 1.0f, 0, 1);
    update_matrix(m1, 1.0f, 1, 1);
    update_matrix(m2, 2.0f, 0, 0);
    update_matrix(m2, 1.0f, 1, 1);
    update_matrix(m2, 1.0f, 2, 0);
    update_matrix(m2, 1.0f, 0, 1);
    update_matrix(m2, 1.0f, 1, 0);
    update_matrix(m2, 2.0f, 2, 1);
    Matrix *m = matrix_kronecker_prod(m1, m2);
    print_matrix(m);
    free_matrix(m); free_matrix(m1); free_matrix(m2);
    free(m); free(m1); free(m2);

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("END OF PROGRAM.\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}