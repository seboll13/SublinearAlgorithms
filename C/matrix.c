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

// /**
//  * @brief compute the determinant of a square matrix
//  * 
//  * @param m matrix
//  * @return int value of the det
//  */
// float det(Matrix *m) {
//     assert(m->cols == m->rows);

//     // If one element on the diagonal is zero, return 0
//     for (int i = 0; i < m->rows; i++)
//         if(m->items[i][i] == 0)
//             return 0;

//     float determinant = 0.0f;
//     // Case where matrix is diagonal
//     if (check_diagonality(m)) {
//         for (int i = 0; i < m->rows; i++)
//             determinant *= m->items[i][i];
//         return determinant;
//     }
//     // Case in which either an entire row or an entire column is zero
//     if (check_entire_line_of_zeroes(m))
//         return 0.0f;
//     return determinant;
// }

// /**
//  * @brief check if a matrix is symmetric
//  * 
//  * @param m 
//  * @return true 
//  * @return false 
//  */
// bool check_symmetry(Matrix *m) {
//     assert(m->rows == m->cols);

//     for (int i = 0; i < m->rows; i++)
//         for (int j = 0; j < m->cols; j++)
//             if (m->items[i][j] != m->items[j][i])
//                 return false;
//     return true;
// }

// /**
//  * @brief Check if the matrix is diagonal
//  * 
//  * @param m matrix
//  * @return true if all elements off-diagonal are zero
//  * @return false otherwise
//  */
// bool check_diagonality(Matrix *m) {
//     assert(m->rows > 0 && m->cols > 0);
    
//     for (int i = 0; i < m->rows; i++)
//         for (int j = 0; j < m->cols; j++)
//             // check the off-diagonal elements
//             if (i != j && m->items[i][j] != 0)
//                 return false;
//     return true;
// }

// /**
//  * @brief check if an entire row or column is made of only zeroes
//  * 
//  * @param m matrix
//  * @return true if at least one row or one col is made of only zeroes
//  * @return false otherwise
//  */
// bool check_entire_line_of_zeroes(Matrix *m) {
//     assert(m->rows > 0 && m->cols > 0);
    
//     bool ret_value;
//     for (int i = 0; i < m->rows; i++) {
//         int row_sum = 0; int col_sum = 0;

//         for (int j = 0; j < m->cols; j++) {
//             row_sum += m->items[i][j]; // sum over each row
//             col_sum += m->items[j][i]; // sum over each line
//         }
//         ret_value = (row_sum == 0 || col_sum == 0);
//     }
//     return ret_value;
// }

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