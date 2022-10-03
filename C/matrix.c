#include "matrix.h"

/**
 * @brief Initialise a new matrix full of zeros
 * 
 * @param m matrix to initialise
 * @param name matrix id
 * @param rows number of rows
 * @param cols number of columns
 */
void init_matrix(Matrix *m, char *name, int rows, int cols) {
    assert(rows > 0 && cols > 0);

    m->rows = rows;
    m->cols = cols;
    m->name = name;
    m->items = malloc(cols * sizeof(Vector));
    
    for (int i = 0; i < cols; i++)
        init_vector(m->items+i, "V", rows);
}

/**
 * @brief Remove current matrix
 * 
 * @param m matrix
 */
void free_matrix(Matrix *m) {
    assert(m->items != NULL);
    if (m != NULL)
        for (int i = 0; i < m->cols; i++)
            free_vector(m->items+i);
    return;
}

/**
 * @brief Update a matrix element
 * 
 * @param m matrix
 * @param n element to add
 * @param row idx of row
 * @param col idx of column
 */
void update_matrix(Matrix *m, float _Complex n, int row, int col) {
    assert(row > -1 && row < (int) sqrt(MAX_MATRIX_CAPACITY));
    assert(col > -1 && col < (int) sqrt(MAX_MATRIX_CAPACITY));

    m->items[col].items[row] = n;
}

/**
 * @brief Create a random matrix containing +/- 1 with equal probability
 * 
 * @param rows # of rows
 * @param cols # of columns
 * @return Matrix* the resulting matrix
 */
Matrix *rademacher_matrix(int rows, int cols) {
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            update_matrix(m, (rand() % 2) ? 1.0f + 0.0f * I : -1.0f + 0.0f * I, i, j);
    return m;
}

/**
 * @brief element-wise addition of two matrices
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @param add indicator of addition
 * @return Matrix addition if sub is false, otherwise subtraction
 */
Matrix *matrix_add(Matrix *m1, Matrix *m2, bool add) {
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

/**
 * @brief return the multiplication of a matrix by some scalar
 * 
 * @param n scalar value
 * @param m matrix
 * @return Matrix* scaled matrix 
 */
Matrix *matrix_scalar_mult(float _Complex n, Matrix *m) {
    Matrix *scaled = malloc(sizeof(Matrix));
    init_matrix(scaled, "MS", m->rows, m->cols);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            scaled->items[j].items[i] = n * m->items[j].items[i];
    return scaled;
}

/**
 * @brief return the multiplication of two matrices together
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @param div indicator of division
 * @return resultant matrix
 */
Matrix *matrix_mult(Matrix *m1, Matrix *m2) {
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

/**
 * @brief return the power of a matrix
 * 
 * @param m matrix
 * @param p power
 * @return Matrix* the matrix m multiplied by itself p times 
 */
Matrix *matrix_power(Matrix *m, int p) {
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

/**
 * @brief return the Hadamard product of two matrices (i.e. element-wise multiplication)
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @return Matrix* resultant matrix
 */
Matrix *matrix_hadamard_prod(Matrix *m1, Matrix *m2) {
    assert(m1->rows == m2->rows && m1->cols == m2->cols);

    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows, m1->cols);

    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            update_matrix(m, m1->items[j].items[i] * m2->items[j].items[i], i, j);
    return m;
}

/**
 * @brief return the Kronecker product of two matrices
 * 
 * @param m1 matrix 1
 * @param m2 matrix 2
 * @return Matrix*
 */
Matrix *matrix_kronecker_prod(Matrix *m1, Matrix *m2) {
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", m1->rows*m2->rows, m1->cols*m2->cols);
    for (int j = 0; j < m->cols; j++) {
        for (int i = 0; i < m->rows; i++) {
            update_matrix(m, matrix_scalar_mult(m1->items[j / m2->cols].items[i / m2->rows], m2)->items[j % m2->cols].items[i % m2->rows], i, j);
        }
    }
    return matrix_transpose(m);
}

/**
 * @brief compute the transpose of a matrix
 * 
 * @param m matrix
 * @return Matrix* the original matrix the rows and columns of which are permuted 
 */
Matrix *matrix_transpose(Matrix *m) {
    Matrix *t = malloc(sizeof(Matrix));
    init_matrix(t, "T", m->cols, m->rows);

    for (int j = 0; j < t->cols; j++)
        for (int i = 0; i < t->rows; i++)
            t->items[j].items[i] = m->items[i].items[j];
    return t;
}

/**
 * @brief compute the conjugate transpose of a matrix
 * 
 * @param m matrix
 * @return Matrix* the matrix of the complex conjugates, the rows and columns of which are permuted 
 */
Matrix *matrix_conj_transpose(Matrix *m) {
    Matrix *t = malloc(sizeof(Matrix));
    init_matrix(t, "T", m->cols, m->rows);

    for (int j = 0; j < t->cols; j++)
        for (int i = 0; i < t->rows; i++)
            t->items[j].items[i] = conj(m->items[i].items[j]);
    return t;
}

/**
 * @brief compute the cofactor matrix of a matrix
 * 
 * @param m matrix
 * @return Matrix* the matrix of the cofactors
 */
Matrix *matrix_cofactor(Matrix *m) {
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

/**
 * @brief compute the adjoint of a matrix
 * 
 * @param m matrix
 * @return Matrix* the conjugate transposed of the cofactor matrix
 */
// compute the adjugate matrix
Matrix *matrix_adjoint(Matrix *m) {
    return matrix_conj_transpose(matrix_cofactor(m));
}

/**
 * @brief compute the inverse of a matrix
 * 
 * @param m matrix
 * @return Matrix* the inverse of the matrix
 */
Matrix *matrix_inverse(Matrix *m) {
    assert(m->rows == m->cols);
    Matrix *inv = malloc(sizeof(Matrix));
    init_matrix(inv, "I", m->rows, m->cols);

    float det = matrix_determinant(m);
    for (int j = 0; j < inv->cols; j++)
        for (int i = 0; i < inv->rows; i++)
            update_matrix(inv, matrix_adjoint(m)->items[j].items[i] / det, i, j);
    return inv;
}

/**
 * @brief Compute the eigenvalues of a matrix
 * TODO: work on this + test
 * 
 * @param m matrix
 * @return Matrix* the collection of eigenvectors 
 */
Matrix *matrix_eigenvalues(Matrix *m) {
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

/**
 * @brief Compute the determinant of a square matrix m
 * 
 * @param m matrix
 * @return int the value of the determinant
 */
float matrix_determinant(Matrix *m) {
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

/**
 * @brief return the trace of a matrix (sum of all diagonal elements)
 * 
 * @param m matrix
 * @return float resulting sum
 */
float _Complex matrix_trace(Matrix *m) {
    assert(m->rows == m->cols);
    float _Complex trace = 0.0f + 0.0f * I;
    for (int j = 0; j < m->cols; j++)
        trace += m->items[j].items[j];
    return trace;
}

/**
 * @brief Rotate a matrix 90 degrees to the left
 * 
 * @param m matrix
 * @return Matrix* the rotated matrix 
 */
Matrix *matrix_rotate_left(Matrix *m) {
    Matrix *r = malloc(sizeof(Matrix));
    init_matrix(r, "R", m->rows, m->cols);
    if (m->rows != m->cols) return NULL;
    for (int j = 0; j < r->cols; j++)
        for (int i = 0; i < r->rows; i++)
            update_matrix(r, m->items[i].items[r->cols-1-j], i, j);
    return r;
}

/**
 * @brief Rotate a matrix 90 degrees to the right
 * 
 * @param m matrix
 * @return Matrix* the rotated matrix 
 */
Matrix *matrix_rotate_right(Matrix *m) {
    Matrix *r = malloc(sizeof(Matrix));
    init_matrix(r, "R", m->rows, m->cols);
    if (m->rows != m->cols) return NULL;
    for (int j = 0; j < r->cols; j++)
        for (int i = 0; i < r->rows; i++)
            update_matrix(r, m->items[r->rows-1-i].items[j], i, j);
    return r;
}

/**
 * @brief check if a matrix is symmetric
 * 
 * @param m matrix
 * @return true if matrix is symmetric
 * @return false otherwise
 */
bool check_symmetry(Matrix *m) {
    assert(m->rows == m->cols);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            if (m->items[i].items[j] != m->items[j].items[i]) return false;
    return true;
}

/**
 * @brief check if a matrix is diagonal
 * 
 * @param m matrix
 * @return true if the matrix contains non-zero elements only on the diagonal
 * @return false otherwise
 */
bool check_diagonality(Matrix *m) {
    assert(m->rows == m->cols);
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            if (i != j && m->items[i].items[j] != 0) return false;
    return true;
}

/**
 * @brief check if there exists a line of zeroes
 * 
 * @param m matrix
 * @return true if there exists a line of zeroes
 * @return false otherwise
 */
bool check_entire_line_of_zeroes(Matrix *m) {
    for (int j = 0; j < m->cols; j++) {
        bool all_zeroes = true;
        for (int i = 0; i < m->rows; i++)
            if (m->items[i].items[j] != 0) all_zeroes = false;
        if (all_zeroes) return true;
    }
    return false;
}

/**
 * @brief compute the L1 norm of a matrix
 * 
 * @param m matrix
 * @return int largest sum of absolute values w.r.t. __columns__
 */
float matrix_L1_norm(Matrix *m) {
    float max_sum = 0.0f;
    for (int j = 0; j < m->rows; j++) {
        float sum = 0.0f;
        for (int i = 0; i < m->cols; i++)
            sum += complex_abs(m->items[i].items[j]);
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}



/**
 * @brief compute the L-infinity norm of a matrix
 * 
 * @param m matrix
 * @return int largest sum of absolute values w.r.t. __rows__
 */
float matrix_Linf_norm(Matrix *m) {
    float max_sum = 0.0f;
    for (int j = 0; j < m->cols; j++) {
        float sum = 0.0f;
        for (int i = 0; i < m->rows; i++)
            sum += complex_abs(m->items[j].items[i]);
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

/**
 * @brief return the Frobenius norm of a given matrix
 * 
 * @param m matrix
 * @return float root of sum of squares
 */
float matrix_frobenius_norm(Matrix *m) {
    float norm = 0.0f;
    for (int j = 0; j < m->cols; j++)
        for (int i = 0; i < m->rows; i++)
            norm += pow(m->items[j].items[i], 2);
    return sqrt(norm);
}

/**
 * @brief print a matrix
 * 
 * @param m 
 */
void print_matrix(Matrix *m) {
    assert(m->rows > 0 && m->cols > 0);
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