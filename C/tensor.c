#include "tensor.h"

Tensor *init_tensor(int rows, int cols, int depth, char *name) {
    assert(rows > 0 && cols > 0 && depth > 0);

    Tensor *T = malloc(sizeof(Tensor));

    T->rows = rows;
    T->cols = cols;
    T->depth = depth;
    T->name = name;
    T->items = malloc(rows * sizeof(int **));

    for (int i = 0; i < rows; i++) {
        T->items[i] = malloc(cols * sizeof(int *));
        for (int j = 0; j < cols; j++) {
            T->items[i][j] = malloc(depth * sizeof(int));
            for (int k = 0; k < depth; k++)
                T->items[i][j][k] = 0;
        }
    }
    return T;
}