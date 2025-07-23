#ifndef TENSOR_HEADER
#define TENSOR_HEADER

#include "matrix.h"

#define MAX_TENSOR_CAPACITY 10e9

typedef struct Tensor {
    int rows, cols, depth;
    Matrix *items;
    char *name;
}Tensor;

// Tensor type construction
void init_tensor(Tensor *T, char *name, int rows, int cols, int depth);
void free_tensor(Tensor *T);
void update_tensor(Tensor *T, float _Complex n, int row, int col, int depth);

// Basic tensor operations
Tensor *tensor_add(const Tensor *E, const Tensor *F, bool add);
Tensor *tensor_scalar_mult(const Tensor *T, float _Complex n);
Tensor *tensor_mult(const Tensor *E, const Tensor *F);
Tensor *tensor_contract(const Tensor *E, const Tensor *F);
Tensor *tensor_cdp(const Matrix *A, const Matrix *B, const Matrix *C);

#endif