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

// ############################ MATRIX TYPE CONSTRUCTION ###############################

/**
 * @brief Initialise a new matrix full of zeros
 * 
 * @param m matrix to initialise
 * @param name matrix id
 * @param rows number of rows
 * @param cols number of columns
 */
void init_matrix(Matrix *m, char *name, int rows, int cols);

/**
 * @brief Remove current matrix
 * 
 * @param m matrix
 */
void free_matrix(Matrix *m);

/**
 * @brief Update a matrix element
 * 
 * @param m matrix
 * @param n element to add
 * @param row idx of row
 * @param col idx of column
 */
void update_matrix(Matrix *m, float _Complex n, int row, int col);

// ################################ MATRIX POPULATION ##################################

/**
 * @brief Create a random matrix containing +/- 1 with equal probability
 * 
 * @param rows # of rows
 * @param cols # of columns
 * @return Matrix* the resulting matrix
 */
Matrix *rademacher_matrix(int rows, int cols);

// ############################ MATRIX OPERATIONS ####################################

/**
 * @brief element-wise addition of two matrices
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @param add indicator of addition
 * @return Matrix addition if sub is false, otherwise subtraction
 */
Matrix *matrix_add(const Matrix *m1, const Matrix *m2, bool add);

/**
 * @brief return the multiplication of a matrix by some scalar
 * 
 * @param n scalar value
 * @param m matrix
 * @return Matrix* scaled matrix 
 */
Matrix *matrix_scalar_mult(float _Complex n, const Matrix *m);

/**
 * @brief return the multiplication of two matrices together
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @param div indicator of division
 * @return resultant matrix
 */
Matrix *matrix_mult(const Matrix *m1, const Matrix *m2);

/**
 * @brief Helper function for fast matrix multiplication, wherein a submatrix is created
 * from a matrix m
 * 
 * @param m matrix
 * @param row_start starting row index
 * @param row_end ending row index
 * @param col_start starting column index
 * @param col_end ending column index
 * @return Matrix* the resulting submatrix
 */
Matrix *create_submatrix(const Matrix *m, int row_start, int row_end, int col_start, int col_end);

/**
 * @brief Helper function for fast matrix multiplication, wherein a submatrix is set
 * into a matrix m
 * 
 * @param m matrix
 * @param sub submatrix to set
 * @param row_start starting row index
 * @param row_end ending row index
 * @param col_start starting column index
 * @param col_end ending column index
 * @return Matrix* the resulting matrix with the submatrix set
 */
Matrix *set_submatrix(Matrix *m, const Matrix *sub, int row_start, int row_end, int col_start, int col_end);

/**
 * @brief return the multiplication of two matrices together using Strassen's algorithm
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @return resultant matrix
 */
Matrix *fast_matrix_mult(const Matrix *m1, const Matrix *m2);

/**
 * @brief return the power of a matrix
 * 
 * @param m matrix
 * @param p power
 * @return Matrix* the matrix m multiplied by itself p times 
 */
Matrix *matrix_power(const Matrix *m, int p);

/**
 * @brief return the Hadamard product of two matrices (i.e. element-wise multiplication)
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @return Matrix* resultant matrix
 */
Matrix *matrix_hadamard_prod(const Matrix *m1, const Matrix *m2);

/**
 * @brief return the Kronecker product of two matrices
 * 
 * @param m1 matrix 1
 * @param m2 matrix 2
 * @return Matrix*
 */
Matrix *matrix_kronecker_prod(const Matrix *m1, const Matrix *m2);

/**
 * @brief return the tensor product of two vectors
 * 
 * @param u 1st vector
 * @param v 2nd vector
 * @return Matrix* resulting element-wise multiplication of the two vectors
 */
Matrix *vector_tensor_prod(const Vector *u, const Vector *v);

/**
 * @brief compute the transpose of a matrix
 * 
 * @param m matrix
 * @return Matrix* the original matrix the rows and columns of which are permuted 
 */
Matrix *matrix_transpose(const Matrix *m);

/**
 * @brief compute the conjugate transpose of a matrix
 * 
 * @param m matrix
 * @return Matrix* the matrix of the complex conjugates, the rows and columns of which are permuted 
 */
Matrix *matrix_conj_transpose(const Matrix *m);

/**
 * @brief compute the cofactor matrix of a matrix
 * 
 * @param m matrix
 * @return Matrix* the matrix of the cofactors
 */
Matrix *matrix_cofactor(const Matrix *m);

/**
 * @brief compute the adjoint of a matrix
 * 
 * @param m matrix
 * @return Matrix* the conjugate transposed of the cofactor matrix
 */
Matrix *matrix_adjoint(const Matrix *m);

/**
 * @brief compute the inverse of a matrix
 * 
 * @param m matrix
 * @return Matrix* the inverse of the matrix
 */
Matrix *matrix_inverse(const Matrix *m);

/**
 * @brief Compute the eigenvalues of a matrix
 * TODO: work on this + test
 * 
 * @param m matrix
 * @return Matrix* the collection of eigenvectors 
 */
Matrix *matrix_eigenvalues(const Matrix *m);

/**
 * @brief Compute the determinant of a square matrix m
 * 
 * @param m matrix
 * @return int the value of the determinant
 */
float matrix_determinant(const Matrix *m);

/**
 * @brief return the trace of a matrix (sum of all diagonal elements)
 * 
 * @param m matrix
 * @return float resulting sum
 */
float _Complex matrix_trace(const Matrix *m);

// ########################### OTHER MATRIX FUNCTIONS ##################################

/**
 * @brief Rotate a matrix 90 degrees to the left
 * 
 * @param m matrix
 * @return Matrix* the rotated matrix 
 */
Matrix *matrix_rotate_left(const Matrix *m);

/**
 * @brief Rotate a matrix 90 degrees to the right
 * 
 * @param m matrix
 * @return Matrix* the rotated matrix 
 */
Matrix *matrix_rotate_right(const Matrix *m);

// ############################### MATRIX PROPERTIES ###################################
/**
 * @brief check if a matrix is symmetric
 * 
 * @param m matrix
 * @return true if matrix is symmetric
 * @return false otherwise
 */
bool matrix_is_symmetric(const Matrix *m);

/**
 * @brief check if a matrix is diagonal
 * 
 * @param m matrix
 * @return true if the matrix contains non-zero elements only on the diagonal
 * @return false otherwise
 */
bool matrix_is_diagonal(const Matrix *m);

/**
 * @brief check if there exists a line of zeroes
 * 
 * @param m matrix
 * @return true if there exists a line of zeroes
 * @return false otherwise
 */
bool matrix_contains_line_of_all_zeroes(const Matrix *m);

/**
 * @brief check that every element in the matrix is an integer
 * 
 * @param m matrix
 * @return true if no element is a float
 * @return false otherwise
 */
bool matrix_is_integral(Matrix *m);

/**
 * @brief check that every element in the matrix is real
 * 
 * @param m matrix
 * @return true if no element is complex
 * @return false otherwise
 */
bool matrix_is_real(Matrix *m);

/**
 * @brief check if a matrix is stochastic
 * 
 * @param m matrix
 * @return true if every line in m sums to 1
 * @return false otherwise
 */
bool matrix_is_stochastic(Matrix *m);

/**
 * @brief check if a matrix is doubly stochastic
 * 
 * @param m matrix
 * @return true if every line and column in m sum to 1
 * @return false otherwise
 */
bool matrix_is_doubly_stochastic(Matrix *m);

/**
 * @brief check if a matrix is a Vandermonde matrix
 * 
 * @param m matrix
 * @return true if every column is a power of the first column
 * @return false otherwise
 */
bool matrix_is_vandermonde(Matrix *m);

// ################################# MATRIX NORMS ######################################

/**
 * @brief compute the L1 norm of a matrix
 * 
 * @param m matrix
 * @return int largest sum of absolute values w.r.t. __columns__
 */
float matrix_L1_norm(const Matrix *m);

/**
 * @brief compute the L-infinity norm of a matrix
 * 
 * @param m matrix
 * @return int largest sum of absolute values w.r.t. __rows__
 */
float matrix_Linf_norm(const Matrix *m);

/**
 * @brief return the Frobenius norm of a given matrix
 * 
 * @param m matrix
 * @return float root of sum of squares
 */
float matrix_frobenius_norm(const Matrix *m);

// ############################# MATRIX PRINTING #######################################

/**
 * @brief Print any kind of matrix
 * 
 * @param m 
 */
void print_matrix(Matrix *m);

/**
 * @brief print an integer matrix
 * 
 * @param m 
 */
void print_integer_matrix(Matrix *m);

/**
 * @brief print a real matrix
 * 
 * @param m 
 */
void print_real_matrix(Matrix *m);

/**
 * @brief Print a complex matrix
 * 
 * @param m 
 */
void print_complex_matrix(Matrix *m);

#endif