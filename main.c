#include <time.h>
#include "vector.h"
#include "matrix.h"

#define DIM 3
#define ROWS 3
#define COLS 3
#define MAX_RND_COMPLEX 10

/**
 * @brief Generates a number between 0 and 1 uniformly at random
 * For the vector to be sparse, generate p to be at most 1/2
 * 
 * @return float 
 */
float generate_probability() {
    return (float) rand() / (float) RAND_MAX;
}

int sign() {
    return rand()%2*2-1;
}

float _Complex generate_random_complex() {
    float re = (float) generate_probability() * MAX_RND_COMPLEX * sign();
    float im = (float) generate_probability() * MAX_RND_COMPLEX * sign();
    return re + im * I;
}

/**
 * @brief generate a random complex vector (with coefficient values between -10 and 10)
 * 
 * @return Vector 
 */
Vector *generate_random_vector() {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", DIM);
    
    for (int i = 0; i < DIM; i++)
        update_vector(v, generate_random_complex(), i);
    return v;
}

/**
 * @brief generate a random complex matrix (with coefficient values between -10 and 10)
 * 
 * @return Matrix 
 */
Matrix *generate_random_matrix() {
    Matrix *m = init_matrix("M", ROWS, COLS);
    
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            update_matrix(m, generate_random_complex(), i, j);
    return m;
}

int main() {
    printf("Start of program...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for probability generations
    
    // VECTOR PART
    // Vector *u = generate_random_vector();
    // Vector *v = generate_random_vector();
    // Vector *w = vector_add(u, v, 1);

    // print_vector(w);
    
    // free_vector(u);
    // free_vector(v);
    // free_vector(w);

    // MATRIX PART
    Matrix *m = generate_random_matrix();
    Matrix *p = transpose(m);

    //print_matrix(p);

    free_matrix(m);
    free_matrix(p);

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("End of program...\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}