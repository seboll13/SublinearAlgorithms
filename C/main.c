#include <time.h>
#include "projections.h"
#include "matrix.h"

int main() {
    printf("Start of program...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for random generation
    
    // VECTOR PART
    // Vector *u = malloc(sizeof(Vector));
    // init_vector(u, "U", 3);
    // free_vector(u);
    // free(u);

    // MATRIX PART
    Matrix *m1 = malloc(sizeof(Matrix));
    Matrix *m2 = malloc(sizeof(Matrix));
    init_matrix(m1, "M1", 3, 3);
    init_matrix(m2, "M2", 3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            update_matrix(m1, 1.0f, i, j);
            update_matrix(m2, 2.0f, i, j);
        }
    }
    Matrix *m = matrix_add(m1, m2, true);
    free_matrix(m); free_matrix(m1); free_matrix(m2);
    free(m); free(m1); free(m2);
    

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("End of program...\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}