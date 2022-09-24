#include "tensor.h"

/**
 * @brief initialise a new tensor full of zeroes
 * 
 * @param T tensor to initialise
 * @param name tensor id
 * @param rows number of rows
 * @param cols number of columns
 * @param depth number of superposed matrices
 */
void init_tensor(Tensor *T, char *name, int rows, int cols, int depth) {
    assert(rows > 0 && cols > 0 && depth > 0);
    
    T->rows = rows;
    T->cols = cols;
    T->depth = depth;
    T->name = name;
    T->items = malloc(depth * sizeof(Matrix));
    for (int i = 0; i < depth; i++)
        init_matrix(T->items+i, "M", rows, cols);
}

/**
 * @brief Remove current tensor
 * 
 * @param T tensor
 */
void free_tensor(Tensor *T) {
    assert(T->items != NULL);
    if (T != NULL)
        for (int i = 0; i < T->depth; i++)
            free_matrix(T->items+i);
    return;
}

/**
 * @brief update a tensor element
 * 
 * @param T tensor
 * @param n element to add
 * @param row index of row
 * @param col index of column
 * @param depth index of depth
 */
void update_tensor(Tensor *T, float _Complex n, int row, int col, int depth) {
    assert(row > -1 && row < (int) pow(MAX_TENSOR_CAPACITY, 1.f/3));
    assert(col > -1 && col < (int) pow(MAX_TENSOR_CAPACITY, 1.f/3));
    assert(depth > -1 && depth < (int) pow(MAX_TENSOR_CAPACITY, 1.f/3));

    T->items[depth].items[col].items[row] = n;
}

/**
 * @brief element-wise addition of two tensors
 * 
 * @param E first tensor
 * @param F second tensor
 * @param add boolean to indicate addition
 * @return Tensor* addition if sub is false, otherwise subtraction
 */
Tensor *tensor_add(Tensor *E, Tensor *F, bool add) {
    assert(E->rows == F->rows && E->cols == F->cols && E->depth == F->depth);

    Tensor *T = malloc(sizeof(Tensor));
    init_tensor(T, "T", E->rows, E->cols, E->depth);

    for (int n_3 = 0; n_3 < T->depth; n_3++)
        for (int n_2 = 0; n_2 < T->cols; n_2++)
            for (int n_1 = 0; n_1 < T->rows; n_1++)
                if (add)
                    update_tensor(T, E->items[n_3].items[n_2].items[n_1] + F->items[n_3].items[n_2].items[n_1], n_1, n_2, n_3);
                else
                    update_tensor(T, E->items[n_3].items[n_2].items[n_1] - F->items[n_3].items[n_2].items[n_1], n_1, n_2, n_3);
    return T;
}

/**
 * @brief return the multiplication of a tensor by some scalar
 * 
 * @param T tensor
 * @param n scalar value
 * @return Tensor* resulting scaled tensor
 */
Tensor *tensor_scalar_mult(Tensor *T, float _Complex n) {
    Tensor *V = malloc(sizeof(Tensor));
    init_tensor(V, "V", T->rows, T->cols, T->depth);
    for (int n_3 = 0; n_3 < V->depth; n_3++)
        for (int n_2 = 0; n_2 < V->cols; n_2++)
            for (int n_1 = 0; n_1 < V->rows; n_1++)
                update_tensor(V, n * T->items[n_3].items[n_2].items[n_1], n_1, n_2, n_3);
    return V;
}

/**
 * @brief element-wise multiplication of two tensors
 * 
 * @param E first tensor
 * @param F second tensor
 * @return Tensor* multiplication
 */
Tensor *tensor_mult(Tensor *E, Tensor *F) {
    assert(E->rows == F->rows && E->cols == F->cols && E->depth == F->depth);

    Tensor *T = malloc(sizeof(Tensor));
    init_tensor(T, "T", E->rows, E->cols, E->depth);

    for (int n_3 = 0; n_3 < T->depth; n_3++)
        for (int n_2 = 0; n_2 < T->cols; n_2++)
            for (int n_1 = 0; n_1 < T->rows; n_1++)
                update_tensor(T, E->items[n_3].items[n_2].items[n_1] * F->items[n_3].items[n_2].items[n_1], n_1, n_2, n_3);
    return T;
}

/**
 * @brief return the canonical polyadic decomposition of a tensor
 * TODO: test
 * 
 * @param A first matrix
 * @param B second matrix
 * @param C third matrix
 * @return Tensor* resulting tensor
 */
Tensor *tensor_cdp(Matrix *A, Matrix *B, Matrix *C) {
    Tensor *V = malloc(sizeof(Tensor));
    init_tensor(V, "V", A->rows, B->rows, C->rows);
    for (int n_3 = 0; n_3 < V->depth; n_3++)
        for (int n_2 = 0; n_2 < V->cols; n_2++)
            for (int n_1 = 0; n_1 < V->rows; n_1++)
                for (int n = 0; n < A->cols; n++)
                    update_tensor(V, A->items[n].items[n_1]*B->items[n].items[n_2]*C->items[n].items[n_3], n_1, n_2, n_3);
    return V;
}