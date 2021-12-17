#include "vector.h"

/**
 * @brief Initialise a vector
 * 
 * @param v vector to initialise
 * @param name vector id
 * @param rows number of rows the vector will have
 */
void init_vector(Vector *v, char *name, int rows) {
    assert(rows > 0);

    v->capacity = rows;
    v->items = malloc(rows * sizeof(int));
    v->name = name;

    // Set the vector to all zeros by default
    for (int i = 0; i < rows; i++)
        v->items[i] = 0;
}

/**
 * @brief Remove vector
 * 
 * @param v vector to be removed
 */
void free_vector(Vector *v) {
    v->capacity = 0;
    v->items = NULL;
    v->name = NULL;
    //free(v);
}

/**
 * @brief Add an element to a vector
 * 
 * @param v vector to update
 * @param n element to add
 * @param idx position at which n is to be added
 */
void add_to_vector(Vector *v, int n, int idx) {
    assert(idx > -1);
    assert(idx < MAX_VEC_CAPACITY);
    
    v->items[idx] = n;
}

/**
 * @brief Print vector elements to screen
 * 
 * @param v vector to be printed
 */
void print_vector(Vector *v) {
    assert(v->capacity > 0);
    
    printf("%s = (", v->name);
    for (int i = 0; i < v->capacity; i++) {
        if (i == v->capacity - 1)
            printf("%d", v->items[i]);
        else
            printf("%d, ", v->items[i]);
    }
    printf(")\n");
}

