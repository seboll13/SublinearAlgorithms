#include "matrix.h"

/**
 * @brief Initialise a new matrix full of zeros
 * 
 * @param m matrix to initialise
 * @param name matrix id
 * @param rows number of rows
 * @param cols number of columns
 */
Matrix *init_matrix(char *name, int rows, int cols) {
    assert(rows > 0 && cols > 0);
    
    Matrix *m = malloc(sizeof(Matrix));

    m->rows = rows;
    m->cols = cols;
    m->name = name;
    m->items = malloc(rows * sizeof(int *));
    
    for (int i = 0; i < rows; i++) {
        m->items[i] = malloc(cols * sizeof(int));
        for (int j = 0; j < rows; j++)
            m->items[i][j] = 0;
    }
    return m;
}

/**
 * @brief Remove current matrix
 * 
 * @param m matrix
 */
void free_matrix(Matrix *m) {
    if (m != NULL) {
        for (int i = 0; i < m->rows; i++)
            free(m->items[i]);
        free(m->items);
        m->rows = 0;
        m->cols = 0;
        m->name = NULL;
        free(m);
    }
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
    assert(row > -1 && row < MAX_MATRIX_CAPACITY);
    assert(col > -1 && col < MAX_MATRIX_CAPACITY);

    m->items[row][col] = n;
}

/**
 * @brief Element wise addition of two matrices
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @param add indicator of addition
 * @return Matrix addition if sub is false, otherwise subtraction
 */
Matrix *matrix_add(Matrix *m1, Matrix *m2, bool add) {
    Matrix *m = init_matrix("M", m1->rows, m1->cols);
    
    for (int i = 0; i < m->rows; i++)
        for (int j = 0; j < m->cols; j++)
            if (add)
                update_matrix(m, m1->items[i][j] + m2->items[i][j], i, j);
            else
                update_matrix(m, m1->items[i][j] - m2->items[i][j], i, j);
    
    return m;
}

/**
 * @brief Returns the multiplication of two matrices together
 * 
 * @param m1 first matrix
 * @param m2 second matrix
 * @param div indicator of division
 * @return Resultant matrix
 */
Matrix *matrix_mult(Matrix *m1, Matrix *m2) {
    // Inner dimensions must be the same
    assert(m1->cols == m2->rows);
    
    Matrix *m = init_matrix("S", m1->rows, m2->cols);
    
    // Perform standard matrix multiplication algorithm
    for (int i = 0; i < m1->rows; i++)
        for (int j = 0; j < m2->cols; j++)
            for (int k = 0; k < m2->rows; k++)
                // We do not use update_matrix() since we must increase the existing value instead of replacing it
                m->items[i][k] += m1->items[i][j] * m2->items[j][k];
    return m;
}

Matrix *transpose(Matrix *m) {
    Matrix *t = init_matrix("T", m->cols, m->rows);

    for (int i = 0; i < t->rows; i++)
        for (int j = 0; j < t->cols; j++)
            t->items[i][j] = m->items[j][i];
    return t;
}

/**
 * @brief check if a matrix is symmetric
 * 
 * @param m 
 * @return true 
 * @return false 
 */
bool check_symmetry(Matrix *m) {
    assert(m->rows == m->cols);

    for (int i = 0; i < m->rows; i++)
        for (int j = 0; j < m->cols; j++)
            if (m->items[i][j] != m->items[j][i])
                return false;
    return true;
}

/**
 * @brief Check if the matrix is diagonal
 * 
 * @param m matrix
 * @return true if all elements off-diagonal are zero
 * @return false otherwise
 */
bool check_diagonality(Matrix *m) {
    assert(m->rows > 0 && m->cols > 0);
    
    for (int i = 0; i < m->rows; i++)
        for (int j = 0; j < m->cols; j++)
            // check the off-diagonal elements
            if (i != j && m->items[i][j] != 0)
                return false;
    return true;
}

/**
 * @brief check if an entire row or column is made of only zeroes
 * 
 * @param m matrix
 * @return true if at least one row or one col is made of only zeroes
 * @return false otherwise
 */
bool check_entire_line_of_zeroes(Matrix *m) {
    assert(m->rows > 0 && m->cols > 0);
    
    bool ret_value;
    for (int i = 0; i < m->rows; i++) {
        int row_sum = 0; int col_sum = 0;

        for (int j = 0; j < m->cols; j++) {
            row_sum += m->items[i][j]; // sum over each row
            col_sum += m->items[j][i]; // sum over each line
        }
        ret_value = (row_sum == 0 || col_sum == 0);
    }
    return ret_value;
}

/**
 * @brief compute the determinant of a square matrix
 * 
 * @param m matrix
 * @return int value of the det
 */
float det(Matrix *m) {
    assert(m->cols == m->rows);

    // If one element on the diagonal is zero, return 0
    for (int i = 0; i < m->rows; i++)
        if(m->items[i][i] == 0)
            return 0;

    float determinant = 0.0f;
    // Case where matrix is diagonal
    if (check_diagonality(m)) {
        for (int i = 0; i < m->rows; i++)
            determinant *= m->items[i][i];
        return determinant;
    }
    // Case in which either an entire row or an entire column is zero
    if (check_entire_line_of_zeroes(m))
        return 0.0f;
    return determinant;
}

float matrix_L1_Linf_norm_helper(Matrix *m, int x, int y) {
    float max_sum = 0.0f;
    for (int j = 0; j < x; j++) {
        float sum = 0.0f;
        for (int i = 0; i < y; i++)
            sum += sqrt(pow(creal(m->items[i][j]),2) + pow(cimag(m->items[i][j]), 2));
        if (sum > max_sum)
            max_sum = sum;
    }
    return max_sum;
}

/**
 * @brief compute the L1 norm of a matrix
 * 
 * @param m matrix
 * @return int largest sum of absolute values w.r.t. __columns__
 */
float matrix_L1_norm(Matrix *m) {
    return matrix_L1_Linf_norm_helper(m, m->cols, m->rows);
}

/**
 * @brief compute the L-infinity norm of a matrix
 * 
 * @param m matrix
 * @return int largest sum of absolute values w.r.t. __rows__
 */
float matrix_Linf_norm(Matrix *m) {
    return matrix_L1_Linf_norm_helper(m, m->rows, m->cols);
}

/**
 * @brief Print a matrix
 * 
 * @param m 
 */
void print_matrix(Matrix *m) {
    assert(m->rows > 0 && m->cols > 0);

    printf("%s = (\n", m->name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            float re = creal(m->items[i][j]); float im = cimag(m->items[i][j]);
            char sign = (im < 0.0f) ? '-' : '+';
            if (j == m->cols - 1)
                printf("%.3f %c %.3fi", re, sign, fabs(im));
            else
                printf("%.3f %c %.3fi  ", re, sign, fabs(im));
        }
        printf("\n");
    }
    printf(")\n");
}