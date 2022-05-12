#include <time.h>
#include "vector.h"
#include "matrix.h"

#define DIM 3
#define ROWS 3
#define COLS 3

/**
 * @brief Generates a number between 0 and 1 uniformly at random
 * For the vector to be sparse, generate p to be at most 1/2
 * 
 * @return double 
 */
double generate_probability() {
    return (double) rand() / (double) RAND_MAX;
}

/**
 * @brief generate a vector only made of 0s and 1s
 * 
 * @param sparse true if we want only a few non zero values
 * @return Vector 
 */
Vector *generate_random_01_vector(bool sparse) {
    Vector *v = NULL;
    init_vector(v, "V", DIM);

    double p = generate_probability();
    if (sparse)
        p /= 2;
    for (int i = 0; i < DIM; i++)
        // W.p. p, we replace the 0 by a 1 in v
        if (generate_probability() < p)
            update_vector(v, 1, i);
    return v;
}

/**
 * @brief generate a matrix only made of 0s and 1s
 * 
 * @param sparse true if we want only a few non zero values
 * @return Matrix 
 */
Matrix *generate_random_01_matrix(bool sparse) {
    Matrix *m = NULL;
    init_matrix(m, "M", ROWS, COLS);

    double p = generate_probability();
    if (sparse)
        p /= 2;
    
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            //W.p. p, replace the 0 by a 1
            if (generate_probability() < p)
                update_matrix(m, 1, i, j);
    return m;
}

int main() {
    srand(time(NULL)); // Set a different seed for probability generations
    
    Vector *v = generate_random_01_vector(true);
    Matrix *m1 = generate_random_01_matrix(false);
    Matrix *m2 = generate_random_01_matrix(false);
    Matrix *n = matrix_mult(m1, m2);
    
    print_vector(v);
    print_matrix(n);

    free_vector(v);
    free_matrix(m1);
    free_matrix(m2);
    free_matrix(n);
}