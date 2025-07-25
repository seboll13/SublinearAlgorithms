#include "tensor.h"

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

void free_tensor(Tensor *T) {
    assert(T->items != NULL);
    if (T != NULL)
        for (int i = 0; i < T->depth; i++)
            free_matrix(T->items+i);
    return;
}

void update_tensor(Tensor *T, float _Complex n, int row, int col, int depth) {
    assert(row > -1 && row < (int) pow(MAX_TENSOR_CAPACITY, 1.f/3));
    assert(col > -1 && col < (int) pow(MAX_TENSOR_CAPACITY, 1.f/3));
    assert(depth > -1 && depth < (int) pow(MAX_TENSOR_CAPACITY, 1.f/3));

    T->items[depth].items[col].items[row] = n;
}

Tensor *tensor_add(const Tensor *E, const Tensor *F, bool add) {
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

Tensor *tensor_scalar_mult(const Tensor *T, float _Complex n) {
    Tensor *V = malloc(sizeof(Tensor));
    init_tensor(V, "V", T->rows, T->cols, T->depth);
    for (int n_3 = 0; n_3 < V->depth; n_3++)
        for (int n_2 = 0; n_2 < V->cols; n_2++)
            for (int n_1 = 0; n_1 < V->rows; n_1++)
                update_tensor(V, n * T->items[n_3].items[n_2].items[n_1], n_1, n_2, n_3);
    return V;
}

Tensor *tensor_mult(const Tensor *E, const Tensor *F) {
    assert(E->rows == F->rows && E->cols == F->cols && E->depth == F->depth);

    Tensor *T = malloc(sizeof(Tensor));
    init_tensor(T, "T", E->rows, E->cols, E->depth);

    for (int n_3 = 0; n_3 < T->depth; n_3++)
        for (int n_2 = 0; n_2 < T->cols; n_2++)
            for (int n_1 = 0; n_1 < T->rows; n_1++)
                update_tensor(T, E->items[n_3].items[n_2].items[n_1] * F->items[n_3].items[n_2].items[n_1], n_1, n_2, n_3);
    return T;
}

Tensor *tensor_contract(const Tensor *E, const Tensor *F) {
    assert(E->cols == F->rows && E->depth == F->depth);

    Tensor *T = malloc(sizeof(Tensor));
    init_tensor(T, "T", E->rows, F->cols, E->depth);

    for (int n_3 = 0; n_3 < T->depth; n_3++)
        for (int n_2 = 0; n_2 < T->cols; n_2++)
            for (int n_1 = 0; n_1 < T->rows; n_1++)
                for (int n = 0; n < E->cols; n++)
                    update_tensor(T, E->items[n_3].items[n].items[n_1] * F->items[n_3].items[n_2].items[n], n_1, n_2, n_3);
    return T;
}

Tensor *tensor_cdp(const Matrix *A, const Matrix *B, const Matrix *C) {
    Tensor *V = malloc(sizeof(Tensor));
    init_tensor(V, "V", A->rows, B->rows, C->rows);
    for (int n_3 = 0; n_3 < V->depth; n_3++)
        for (int n_2 = 0; n_2 < V->cols; n_2++)
            for (int n_1 = 0; n_1 < V->rows; n_1++)
                for (int n = 0; n < A->cols; n++)
                    update_tensor(V, A->items[n].items[n_1]*B->items[n].items[n_2]*C->items[n].items[n_3], n_1, n_2, n_3);
    return V;
}