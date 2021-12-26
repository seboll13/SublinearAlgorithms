#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "libs.h"

#define MAX_MATRIX_CAPACITY 1000

typedef struct Matrix {
    int rows, cols;
    int **items;
    char *name;
} Matrix;

// Matrix type construction
void init_matrix(Matrix *m, char *name, int rows, int cols);
void free_matrix(Matrix *m);
void update_matrix(Matrix *m, int n, int row, int col);

// Printing part
void print_matrix(Matrix *m);

#endif