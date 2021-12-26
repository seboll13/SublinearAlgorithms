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
    assert(rows > 0);
    assert(cols > 0);

    m->rows = rows;
    m->cols = cols;
    m->name = name;
    m->items = malloc(rows * sizeof(int *));

    // Set the matrix to all zeros by default
    for (int i = 0; i < rows; i++)
        m->items[i] = malloc(cols * sizeof(int));
    
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < rows; j++)
            m->items[i][j] = 0;
}

/**
 * @brief Remove current matrix
 * 
 * @param m matrix
 */
void free_matrix(Matrix *m) {
    m->rows = 0;
    m->cols = 0;
    m->name = NULL;
}

/**
 * @brief Update a matrix element
 * 
 * @param m matrix
 * @param n element to add
 * @param row idx of row
 * @param col idx of column
 */
void update_matrix(Matrix *m, int n, int row, int col) {
    assert(row > -1 && row < MAX_MATRIX_CAPACITY);
    assert(col > -1 && col < MAX_MATRIX_CAPACITY);

    m->items[row][col] = n;
}

/**
 * @brief Print a matrix
 * 
 * @param m 
 */
void print_matrix(Matrix *m) {
    assert(m->rows > -1 && m->cols > -1);

    printf("%s = (\n", m->name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++)
            if (j == m->cols - 1)
                printf("%d", m->items[i][j]);
            else
                printf("%d ", m->items[i][j]);
        printf("\n");
    }
    printf(")\n");
}