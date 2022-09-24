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
Tensor *tensor_add(Tensor *E, Tensor *F, bool add);
Tensor *tensor_mult(Tensor *E, Tensor *F);
Tensor *tensor_scalar_mult(Tensor *T, float _Complex n);

#endif