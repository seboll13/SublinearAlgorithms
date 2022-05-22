#include <time.h>
#include "projections.h"
#include "matrix.h"

int main() {
    printf("Start of program...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for random generation
    
    // VECTOR PART
    Vector *u = malloc(sizeof(Vector));
    init_vector(u, "U", 3);
    free_vector(u);
    free(u);

    // MATRIX PART
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 3);
    
    for (int i = 0; i < 3; i++)
        print_vector(m->items+i);
    free_matrix(m);
    free(m);

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("End of program...\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}