#include <time.h>
#include "vector.h"

#define DIM 30

/**
 * @brief Generates a number between 0 and 1 uniformly at random
 * For the vector to be sparse, generate p to be at most 1/2
 * 
 * @return double 
 */
double generate_probability() {
    return (double) rand() / (double) RAND_MAX;
}

int main() {
    Vector u, v;

    init_vector(&u, "U", DIM);
    init_vector(&v, "V", DIM);

    srand(time(NULL)); // Set a different seed for probability generations
    double p = generate_probability();
    double q = generate_probability();
    printf("p=%f\n", p);
    printf("q=%f\n", q);
    for (int i = 0; i < DIM; i++) {
        // W.p. p, we replace the 0 by a 1 in u
        if (generate_probability() < p/2)
            update_vector(&u, 1, i);
        // W.p. q, we replace the 0 by a 1 in v
        if (generate_probability() < q/2)
            update_vector(&v, 1, i);
    }

    print_vector(&u);
    print_vector(&v);

    free_vector(&u);
    free_vector(&v);
}