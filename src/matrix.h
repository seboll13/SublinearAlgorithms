#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include "vector.h"

#define MAX_MATRIX_DIM (int)1e4
#define THRESHOLD 4

typedef struct Matrix {
    int rows;
    int cols;
    Vector *items;
    char *name;
} Matrix;

// Threading part
#define NUM_THREADS 8

pthread_t threads[NUM_THREADS];
Matrix *results[NUM_THREADS];

void *thread_func(void *arg);

// Matrix type construction
void init_matrix(Matrix *m, char *name, int rows, int cols);
void free_matrix(Matrix *m);
void update_matrix(Matrix *m, float _Complex n, int row, int col);

// Matrix population
Matrix *rademacher_matrix(int rows, int cols);

// Matrix operations
Matrix *matrix_add(const Matrix *m1, const Matrix *m2, bool add);
Matrix *matrix_scalar_mult(float _Complex n, const Matrix *m);
Matrix *matrix_mult(const Matrix *m1, const Matrix *m2);
Matrix *create_submatrix(const Matrix *m, int row_start, int row_end, int col_start, int col_end);
Matrix *set_submatrix(Matrix *m, const Matrix *sub, int row_start, int row_end, int col_start, int col_end);
Matrix *fast_matrix_mult(const Matrix *m1, const Matrix *m2);
Matrix *matrix_power(const Matrix *m, int p);
Matrix *matrix_hadamard_prod(const Matrix *m1, const Matrix *m2);
Matrix *matrix_kronecker_prod(const Matrix *m1, const Matrix *m2);
Matrix *vector_tensor_prod(const Vector *u, const Vector *v);
Matrix *matrix_transpose(const Matrix *m);
Matrix *matrix_conj_transpose(const Matrix *m);
Matrix *matrix_cofactor(const Matrix *m);
Matrix *matrix_adjoint(const Matrix *m);
Matrix *matrix_inverse(const Matrix *m);
Matrix *matrix_eigenvalues(const Matrix *m);
float matrix_determinant(const Matrix *m);
float _Complex matrix_trace(const Matrix *m);

// Other matrix functions
Matrix *matrix_rotate_left(const Matrix *m);
Matrix *matrix_rotate_right(const Matrix *m);

// Helper functions
bool matrix_is_symmetric(const Matrix *m);
bool matrix_is_diagonal(const Matrix *m);
bool matrix_contains_line_of_all_zeroes(const Matrix *m);
bool matrix_is_integral(Matrix *m);
bool matrix_is_real(Matrix *m);
bool matrix_is_stochastic(Matrix *m);
bool matrix_is_doubly_stochastic(Matrix *m);
bool matrix_is_vandermonde(Matrix *m);

// Matrix norms
float matrix_L1_norm(const Matrix *m);
float matrix_Linf_norm(const Matrix *m);
float matrix_frobenius_norm(const Matrix *m);

// Printing part
void print_matrix(Matrix *m);
void print_integer_matrix(Matrix *m);
void print_real_matrix(Matrix *m);
void print_complex_matrix(Matrix *m);

#endif