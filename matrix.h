#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "libs.h"

#define MAX_MATRIX_CAPACITY 10e6

typedef struct Matrix {
    int rows, cols;
    int **items;
    char *name;
} Matrix;

// Matrix type construction
void init_matrix(Matrix *m, char *name, int rows, int cols);
void free_matrix(Matrix *m);
void update_matrix(Matrix *m, int n, int row, int col);

// Matrix operations
Matrix matrix_add(Matrix *m1, Matrix *m2, bool sub);
Matrix matrix_mult(Matrix *m1, Matrix *m2);

// Helper functions
bool check_diagonality(Matrix *m);
bool check_entire_line_of_zeroes(Matrix *m);

// Matrix properties
int det(Matrix *m1);

// Printing part
void print_matrix(Matrix *m);

#endif