#include "matrix.h"

void *thread_func(void *arg) {
    const Matrix *m1 = ((Matrix **)arg)[0];
    const Matrix *m2 = ((Matrix **)arg)[1];
    return (void *)fast_matrix_mult(m1, m2);
}

void init_matrix(Matrix *m, char *name, int rows, int cols) {
    assert(rows > 0 && cols > 0);

    m->rows = rows;
    m->cols = cols;
    m->name = name;
    m->items = malloc(cols * sizeof(Vector));
    
    for (int i = 0; i < cols; i++)
        init_vector(m->items+i, "V", rows);
}

void free_matrix(Matrix *m) {
    assert(m != NULL);
    if (m->items != NULL)
        for (int i = 0; i < m->cols; i++)
            free_vector(m->items+i);
    return;
}

void update_matrix(Matrix *m, float _Complex n, int row, int col) {
    assert(row > -1 && row < MAX_MATRIX_DIM);
    assert(col > -1 && col < MAX_MATRIX_DIM);

    m->items[col].items[row] = n;
}

Matrix *rademacher_matrix(int rows, int cols) {
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            update_matrix(m, (rand() % 2) ? 1.0f + 0.0f * I : -1.0f + 0.0f * I, i, j);
    return m;
}

Matrix *matrix_add(const Matrix *m1, const Matrix *m2, bool add) {
    assert(m1->rows == m2->rows);
    assert(m1->cols == m2->cols);

    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows, m1->cols);
    
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            if (add)
                update_matrix(m, m1->items[j].items[i] + m2->items[j].items[i], i, j);
            else
                update_matrix(m, m1->items[j].items[i] - m2->items[j].items[i], i, j);
    return m;
}

Matrix *matrix_scalar_mult(float _Complex n, const Matrix *m) {
    Matrix *scaled = malloc(sizeof(Matrix));
    init_matrix(scaled, "MS", m->rows, m->cols);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            scaled->items[j].items[i] = n * m->items[j].items[i];
    return scaled;
}

Matrix *matrix_mult(const Matrix *m1, const Matrix *m2) {
    // Inner dimensions must be the same
    assert(m1->cols == m2->rows);
    
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows, m2->cols); // keep outer dimensions
    
    // Perform standard matrix multiplication algorithm
    for (int i = 0; i < m1->rows; i++)
        for (int j = 0; j < m2->cols; j++)
            for (int k = 0; k < m2->rows; k++)
                // We do not use update_matrix() since we must increase the existing value instead of replacing it
                m->items[j].items[i] += m1->items[k].items[i] * m2->items[j].items[k];
    return m;
}

Matrix *create_submatrix(const Matrix *m, int row_start, int row_end, int col_start, int col_end) {
    Matrix *sub = malloc(sizeof(Matrix));
    init_matrix(sub, "M", row_end - row_start, col_end - col_start);
    for (int i = row_start; i < row_end; i++)
        for (int j = col_start; j < col_end; j++)
            update_matrix(sub, m->items[j].items[i], i - row_start, j - col_start);
    return sub;
}

Matrix *set_submatrix(Matrix *m, const Matrix *sub, int row_start, int row_end, int col_start, int col_end) {
    for (int i = row_start; i < row_end; i++)
        for (int j = col_start; j < col_end; j++)
            update_matrix(m, sub->items[j - col_start].items[i - row_start], i, j);
    return m;
}

Matrix *fast_matrix_mult(const Matrix *m1, const Matrix *m2) {
    // Inner dimensions must be the same
    assert(m1->cols == m2->rows);

    // Sanity checks: Strassen's algorithm will not work if the matrix is not a power of 2
    bool is_power_of_2 = !(m1->rows & (m1->rows - 1)) && !(m1->cols & (m1->cols - 1));
    // For 2x2 matrices, call the standard matrix multiplication algorithm
    bool under_threshold = (m1->rows < THRESHOLD) || (m1->cols < THRESHOLD) || (m2->rows < THRESHOLD) || (m2->cols < THRESHOLD);

    if (is_power_of_2 || under_threshold)
        return matrix_mult(m1, m2);
    
    int n = m1->rows;
    int half = n / 2;
    Matrix *A11 = create_submatrix(m1, 0, half, 0, half);
    const Matrix *A12 = create_submatrix(m1, 0, half, half, n);
    const Matrix *A21 = create_submatrix(m1, half, n, 0, half);
    const Matrix *A22 = create_submatrix(m1, half, n, half, n);
    Matrix *B11 = create_submatrix(m2, 0, half, 0, half);
    const Matrix *B12 = create_submatrix(m2, 0, half, half, n);
    Matrix *B21 = create_submatrix(m2, half, n, 0, half);
    Matrix *B22 = create_submatrix(m2, half, n, half, n);

    Matrix *S1 = matrix_add(B12, B22, false);
    Matrix *S2 = matrix_add(A11, A12, true);
    Matrix *S3 = matrix_add(A21, A22, true);
    Matrix *S4 = matrix_add(B21, B11, false);
    Matrix *S5 = matrix_add(A11, A22, true);
    Matrix *S6 = matrix_add(B11, B22, true);
    Matrix *S7 = matrix_add(A12, A22, false);

    // Recursive calls
    Matrix *matrices[3][3] = {{A11, S1}, {S2, B22}, {S3, B11}, {A22, S4}, {S5, S6}, {S7, B21}, {S7, S6}};
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)matrices[i]);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], (void **)&results[i]);
    }

    const Matrix *P1 = results[0];
    const Matrix *P2 = results[1];
    const Matrix *P3 = results[2];
    const Matrix *P4 = results[3];
    const Matrix *P5 = results[4];
    const Matrix *P6 = results[5];
    const Matrix *P7 = results[6];

    const Matrix *C11 = matrix_add(matrix_add(matrix_add(P5, P4, true), P2, false), P6, true);
    const Matrix *C12 = matrix_add(P1, P2, true);
    const Matrix *C21 = matrix_add(P3, P4, true);
    const Matrix *C22 = matrix_add(matrix_add(P5, P1, true), matrix_add(P3, P7, true), false);

    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows, m2->cols); // keep outer dimensions

    set_submatrix(m, C11, 0, half, 0, half);
    set_submatrix(m, C12, 0, half, half, n);
    set_submatrix(m, C21, half, n, 0, half);
    set_submatrix(m, C22, half, n, half, n);
    return m;
}

Matrix *matrix_power(const Matrix *m, int p) {
    assert(m->rows == m->cols);
    Matrix *pow = malloc(sizeof(Matrix));
    init_matrix(pow, "P", m->rows, m->cols);
    for (int j = 0; j < pow->cols; j++)
        for (int i = 0; i < pow->rows; i++)
            update_matrix(pow, i == j ? 1 : 0, i, j);
    for (int i = 0; i < p; i++)
        pow = matrix_mult(pow, m);
    return pow;
}

Matrix *matrix_hadamard_prod(const Matrix *m1, const Matrix *m2) {
    assert(m1->rows == m2->rows && m1->cols == m2->cols);

    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows, m1->cols);

    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            update_matrix(m, m1->items[j].items[i] * m2->items[j].items[i], i, j);
    return m;
}

Matrix *matrix_kronecker_prod(const Matrix *m1, const Matrix *m2) {
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows*m2->rows, m1->cols*m2->cols);
    for (int j = 0; j < m->cols; j++) {
        for (int i = 0; i < m->rows; i++) {
            update_matrix(m, matrix_scalar_mult(m1->items[j / m2->cols].items[i / m2->rows], m2)->items[j % m2->cols].items[i % m2->rows], i, j);
        }
    }
    return matrix_transpose(m);
}

Matrix *vector_tensor_prod(const Vector *u, const Vector *v) {
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", u->capacity, v->capacity);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            update_matrix(m, u->items[i] * v->items[j], i, j);
    return m;
}

Matrix *matrix_transpose(const Matrix *m) {
    Matrix *t = malloc(sizeof(Matrix));
    init_matrix(t, "T", m->cols, m->rows);

    for (int j = 0; j < t->cols; j++)
        for (int i = 0; i < t->rows; i++)
            t->items[j].items[i] = m->items[i].items[j];
    return t;
}

Matrix *matrix_conj_transpose(const Matrix *m) {
    Matrix *t = malloc(sizeof(Matrix));
    init_matrix(t, "T", m->cols, m->rows);

    for (int j = 0; j < t->cols; j++)
        for (int i = 0; i < t->rows; i++)
            t->items[j].items[i] = conj(m->items[i].items[j]);
    return t;
}

Matrix *matrix_cofactor(const Matrix *m) {
    assert(m->rows == m->cols);
    Matrix *c = malloc(sizeof(Matrix));
    init_matrix(c, "C", m->rows, m->cols);

    for (int j = 0; j < c->cols; j++) {
        for (int i = 0; i < c->rows; i++) {
            Matrix *sub = malloc(sizeof(Matrix));
            init_matrix(sub, "S", m->rows-1, m->cols-1);
            for (int k = 0; k < m->rows; k++) {
                if (k == i) continue;
                for (int l = 0; l < m->cols; l++) {
                    if (l == j) continue;
                    update_matrix(sub, m->items[l].items[k], k > i ? k-1 : k, l > j ? l-1 : l);
                }
            }
            update_matrix(c, matrix_determinant(sub) * pow(-1, i+j), i, j);
        }
    }
    return c;
}

Matrix *matrix_adjoint(const Matrix *m) {
    return matrix_conj_transpose(matrix_cofactor(m));
}

Matrix *matrix_inverse(const Matrix *m) {
    assert(m->rows == m->cols);
    Matrix *inv = malloc(sizeof(Matrix));
    init_matrix(inv, "I", m->rows, m->cols);

    float det = matrix_determinant(m);
    for (int j = 0; j < inv->cols; j++)
        for (int i = 0; i < inv->rows; i++)
            update_matrix(inv, matrix_adjoint(m)->items[j].items[i] / det, i, j);
    return inv;
}

Matrix *matrix_eigenvalues(const Matrix *m) {
    assert(m->rows == m->cols);
    Matrix *eig = malloc(sizeof(Matrix));
    init_matrix(eig, "E", m->rows, 1);
    for (int i = 0; i < eig->rows; i++) {
        Matrix *sub = malloc(sizeof(Matrix));
        init_matrix(sub, "S", m->rows-1, m->cols-1);
        for (int j = 0; j < m->rows; j++) {
            if (j == i) continue;
            for (int k = 0; k < m->cols; k++) {
                if (k == 0) continue;
                update_matrix(sub, m->items[j].items[k], j > i ? j-1 : j, k-1);
            }
        }
        update_matrix(eig, matrix_determinant(sub) / m->items[i].items[0], i, 0);
    }
    return eig;
}

float matrix_determinant(const Matrix *m) {
    assert(m->rows == m->cols);
    if (m->rows == 1) return m->items[0].items[0];
    float det = 0.0f;
    for (int i = 0; i < m->rows; i++) {
        Matrix *sub = malloc(sizeof(Matrix));
        init_matrix(sub, "S", m->rows-1, m->cols-1);
        for (int j = 0; j < m->rows; j++) {
            if (j == i) continue;
            for (int k = 0; k < m->cols; k++) {
                if (k == 0) continue;
                update_matrix(sub, m->items[j].items[k], j > i ? j-1 : j, k-1);
            }
        }
        det += m->items[i].items[0] * matrix_determinant(sub) * (i % 2 == 0 ? 1 : -1);
    }
    return det;
}

float _Complex matrix_trace(const Matrix *m) {
    assert(m->rows == m->cols);
    float _Complex trace = 0.0f + 0.0f * I;
    for (int j = 0; j < m->cols; j++)
        trace += m->items[j].items[j];
    return trace;
}

Matrix *matrix_rotate_left(const Matrix *m) {
    Matrix *r = malloc(sizeof(Matrix));
    init_matrix(r, "R", m->rows, m->cols);
    if (m->rows != m->cols) return NULL;
    for (int j = 0; j < r->cols; j++)
        for (int i = 0; i < r->rows; i++)
            update_matrix(r, m->items[i].items[r->cols-1-j], i, j);
    return r;
}

Matrix *matrix_rotate_right(const Matrix *m) {
    Matrix *r = malloc(sizeof(Matrix));
    init_matrix(r, "R", m->rows, m->cols);
    if (m->rows != m->cols) return NULL;
    for (int j = 0; j < r->cols; j++)
        for (int i = 0; i < r->rows; i++)
            update_matrix(r, m->items[r->rows-1-i].items[j], i, j);
    return r;
}

bool matrix_is_symmetric(const Matrix *m) {
    assert(m->rows == m->cols);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            if (m->items[i].items[j] != m->items[j].items[i]) return false;
    return true;
}

bool matrix_is_diagonal(const Matrix *m) {
    assert(m->rows == m->cols);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            if (i != j && m->items[i].items[j] != 0) return false;
    return true;
}

bool matrix_contains_line_of_all_zeroes(const Matrix *m) {
    for (int j = 0; j < m->cols; j++) {
        bool all_zeroes = true;
        for (int i = 0; i < m->rows; i++)
            if (m->items[i].items[j] != 0) all_zeroes = false;
        if (all_zeroes) return true;
    }
    return false;
}

bool matrix_is_integral(Matrix *m) {
    for (int j = 0; j < m->cols; j++)
        if (!vector_is_integral(&m->items[j])) return false;
    return true;
}

bool matrix_is_real(Matrix *m) {
    for (int j = 0; j < m->cols; j++)
        if (!vector_is_real(&m->items[j])) return false;
    return true;
}

bool matrix_is_stochastic(Matrix *m) {
    // For simplicity, check that there is no imaginary part
    assert(matrix_is_real(m));
    // The sum of each line must be 1
    for (int j = 0; j < m->cols; j++) {
        float sum = 0.0f;
        for (int i = 0; i < m->rows; i++) {
            float curr = m->items[i].items[j];
            if (curr < 0.0f) return false;
            sum += curr;
        }
        if (sum != 1.0f) return false;
    }
    return true;
}

bool matrix_is_doubly_stochastic(Matrix *m) {
    if (!matrix_is_stochastic(m)) return false;
    // The sum of each column must be 1
    for (int i = 0; i < m->rows; i++) {
        float sum = 0.0f;
        for (int j = 0; j < m->cols; j++) {
            float curr = m->items[i].items[j];
            if (curr < 0.0f) return false;
            sum += curr;
        }
        if (sum != 1.0f) return false;
    }
    return true;
}

bool matrix_is_vandermonde(Matrix *m) {
    // For simplicity, check that there is no imaginary part
    assert(matrix_is_real(m));
    // The first column must be all 1s
    for (int i = 0; i < m->rows; i++)
        if (m->items[i].items[0] != 1.0f) return false;
    // The other columns must be powers of the first column
    for (int j = 1; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            if (m->items[i].items[j] != powf(m->items[i].items[0], j)) return false;
    return true;
}

float matrix_L1_norm(const Matrix *m) {
    float max_sum = 0.0f;
    for (int j = 0; j < m->rows; j++) {
        float sum = 0.0f;
        for (int i = 0; i < m->cols; i++)
            sum += complex_abs(m->items[i].items[j]);
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

float matrix_Linf_norm(const Matrix *m) {
    float max_sum = 0.0f;
    for (int j = 0; j < m->cols; j++) {
        float sum = 0.0f;
        for (int i = 0; i < m->rows; i++)
            sum += complex_abs(m->items[j].items[i]);
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

float matrix_frobenius_norm(const Matrix *m) {
    float norm = 0.0f;
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            norm += pow(m->items[j].items[i], 2);
    return sqrt(norm);
}

void print_matrix(Matrix *m) {
    assert(m->rows > 0 && m->cols > 0);
    if (matrix_is_integral(m))
        print_integer_matrix(m);
    else if (matrix_is_real(m))
        print_real_matrix(m);
    else
        print_complex_matrix(m);
}

void print_integer_matrix(Matrix *m) {
    printf("%s = (\n", m->name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++)
            printf("  %d", (int) m->items[i].items[j]);
        printf("\n");
    }
    printf(")\n");
}

void print_real_matrix(Matrix *m) {
    printf("%s = (\n", m->name);
    for (int j = 0; j < m->cols; j++) {
        for (int i = 0; i < m->rows; i++) {
            float re = m->items[j].items[i];
            printf("     %.3f", re);
        }
        printf("\n");
    }
    printf(")\n");
}

void print_complex_matrix(Matrix *m) {
    printf("%s = (\n", m->name);
    for (int j = 0; j < m->cols; j++) {
        for (int i = 0; i < m->rows; i++) {
            float re = creal(m->items[j].items[i]); float im = cimag(m->items[j].items[i]);
            char sign = (im < 0.0f) ? '-' : '+';
            printf("     %.3f %c %.3fi", re, sign, fabs(im));
        }
        printf("\n");
    }
    printf(")\n");
}