#ifndef TENSOR_HEADER
#define TENSOR_HEADER

#include "libs.h"
#include "matrix.h"
#include "vector.h"

typedef struct Tensor {
    int rows, cols, depth;
    float _Complex ***items;
    char *name;
}Tensor;

// Tensor type construction
Tensor *init_tensor(int rows, int cols, int depth, char *name);

#endif