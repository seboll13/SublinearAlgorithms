#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "vector.h"

#define MAX_MATRIX_CAPACITY 10e6

typedef struct Matrix {
    int rows, cols;
    Vector *items;
    char *name;
} Matrix;

// Matrix type construction
void init_matrix(Matrix *m, char *name, int rows, int cols);
void free_matrix(Matrix *m);
void update_matrix(Matrix *m, float _Complex n, int row, int col);

// Matrix population
Matrix *rademacher_matrix(int rows, int cols);

// Matrix operations
Matrix *matrix_add(Matrix *m1, Matrix *m2, bool add);
Matrix *matrix_scalar_mult(float _Complex n, Matrix *m);
Matrix *matrix_mult(Matrix *m1, Matrix *m2);
Matrix *matrix_power(Matrix *m, int p);
Matrix *matrix_hadamard_prod(Matrix *m1, Matrix *m2);
Matrix *matrix_kronecker_prod(Matrix *m1, Matrix *m2);
Matrix *matrix_transpose(Matrix *m);
Matrix *matrix_conj_transpose(Matrix *m);
Matrix *matrix_cofactor(Matrix *m);
Matrix *matrix_adjoint(Matrix *m);
Matrix *matrix_inverse(Matrix *m);
Matrix *matrix_eigenvalues(Matrix *m);
float matrix_determinant(Matrix *m);
float _Complex matrix_trace(Matrix *m);

// Other matrix functions
Matrix *matrix_rotate_left(Matrix *m);
Matrix *matrix_rotate_right(Matrix *m);

// Helper functions
bool check_symmetry(Matrix *m);
bool check_diagonality(Matrix *m);
bool check_entire_line_of_zeroes(Matrix *m);

// Matrix norms
float matrix_L1_norm(Matrix *m);
float matrix_Linf_norm(Matrix *m);
float matrix_frobenius_norm(Matrix *m);

// Printing part
void print_matrix(Matrix *m);

#endif