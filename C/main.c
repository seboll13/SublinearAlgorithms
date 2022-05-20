#include <time.h>
#include "projections.h"
#include "matrix.h"

// /**
//  * @brief generate a random complex matrix (with coefficient values between -10 and 10)
//  * 
//  * @return Matrix 
//  */
// Matrix *generate_random_matrix() {
//     Matrix *m = init_matrix("M", ROWS, COLS);
    
//     for (int i = 0; i < ROWS; i++)
//         for (int j = 0; j < COLS; j++)
//             update_matrix(m, generate_random_complex(), i, j);
//     return m;
// }

int main() {
    printf("Start of program...\n");
    clock_t start = clock();
    srand(time(NULL)); // Set a different seed for probability generations

    printf("Res: %d\n", vector_random_projection(100, 0.5));
    
    // VECTOR PART
     Vector *u = malloc(sizeof(Vector));
     init_vector(u, "U", 1);
     free_vector(u);
    // Vector *v = generate_random_vector(3);
    // Vector *w = vector_add(u, v, 1);

    // print_vector(w);
    
    // free_vector(u);
    // free_vector(v);
    // free_vector(w);

    // MATRIX PART
    // Matrix *m = generate_random_matrix();
    // Matrix *p = transpose(m);

    //print_matrix(p);

    // free_matrix(m);
    // free_matrix(p);

    clock_t end = clock();
    double execution_time = (double)(end-start) / CLOCKS_PER_SEC * 1000;

    printf("End of program...\n");
    printf("Total execution time : %.3f [ms]\n", execution_time);
}