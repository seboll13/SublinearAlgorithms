#ifndef TENSOR_HEADER
#define TENSOR_HEADER

#include "matrix.h"

#define MAX_TENSOR_CAPACITY 10e9

typedef struct Tensor {
    int rows, cols, depth;
    Matrix *items;
    char *name;
}Tensor;

// ############################ TENSOR TYPE CONSTRUCTION ###############################

/**
 * @brief initialise a new tensor full of zeroes
 * 
 * @param T tensor to initialise
 * @param name tensor id
 * @param rows number of rows
 * @param cols number of columns
 * @param depth number of superposed matrices
 */
void init_tensor(Tensor *T, char *name, int rows, int cols, int depth);

/**
 * @brief Remove current tensor
 * 
 * @param T tensor
 */
void free_tensor(Tensor *T);

/**
 * @brief update a tensor element
 * 
 * @param T tensor
 * @param n element to add
 * @param row index of row
 * @param col index of column
 * @param depth index of depth
 */
void update_tensor(Tensor *T, float _Complex n, int row, int col, int depth);

// ############################ BASIC TENSOR OPERATIONS ################################

/**
 * @brief element-wise addition of two tensors
 * 
 * @param E first tensor
 * @param F second tensor
 * @param add boolean to indicate addition
 * @return Tensor* addition if sub is false, otherwise subtraction
 */
Tensor *tensor_add(const Tensor *E, const Tensor *F, bool add);

/**
 * @brief return the multiplication of a tensor by some scalar
 * 
 * @param T tensor
 * @param n scalar value
 * @return Tensor* resulting scaled tensor
 */
Tensor *tensor_scalar_mult(const Tensor *T, float _Complex n);

/**
 * @brief element-wise multiplication of two tensors
 * 
 * @param E first tensor
 * @param F second tensor
 * @return Tensor* multiplication
 */
Tensor *tensor_mult(const Tensor *E, const Tensor *F);

/**
 * @brief return the contraction of two tensors
 * TODO: test
 * 
 * @param E first tensor
 * @param F second tensor
 * @return Tensor* resulting tensor
 */
Tensor *tensor_contract(const Tensor *E, const Tensor *F);

/**
 * @brief return the canonical polyadic decomposition of a tensor
 * TODO: test
 * 
 * @param A first matrix
 * @param B second matrix
 * @param C third matrix
 * @return Tensor* resulting tensor
 */
Tensor *tensor_cdp(const Matrix *A, const Matrix *B, const Matrix *C);

#endif