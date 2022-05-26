#include <time.h>
#include "projections.h"
#include "matrix.h"

int main() {
    printf("START OF PROGRAM...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for random generation
    
    // VECTOR PART
    Vector *u = malloc(sizeof(Vector));
    init_vector(u, "U", 3);
    for (int i = 0; i < u->capacity; i++)
        update_vector(u, 1.0f, i);
    print_vector(u);
    free_vector(u);
    free(u);

    // MATRIX PART
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 3);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            update_matrix(m, 1.0f, i, j);
    print_matrix(m);
    free_matrix(m);
    free(m);

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("END OF PROGRAM.\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}