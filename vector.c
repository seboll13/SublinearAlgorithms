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
 * @brief Update a vector element
 * 
 * @param v vector to update
 * @param n element to add
 * @param idx position at which n is to be added
 */
void update_vector(Vector *v, int n, int idx) {
    assert(idx > -1);
    assert(idx < MAX_VEC_CAPACITY);
    
    v->items[idx] = n;
}

/**
 * @brief Computes the sum of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return Vector* u+v
 */
Vector *vector_add(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    Vector *w ;
    init_vector(w, "W", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(w, u->items[i] + v->items[i], i);
    return w;
}

/**
 * @brief Computes the difference between two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return Vector* u-v
 */
Vector *vector_sub(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    Vector *w ;
    init_vector(w, "W", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(w, u->items[i] - v->items[i], i);
    return w;
}

/**
 * @brief Computes the multiplication of a vector by a scalar
 * 
 * @param u vector
 * @param a scalar
 * @return Vector* scaled vector
 */
Vector *scalar_mult(Vector *u, int a) {
    Vector *v;
    init_vector(v, "V", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(v, a * u->items[i], i);
    return v;
}

/**
 * @brief Returns the scalar product of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return int sum of products of each coefficient
 */
int scalar_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);
    int res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += u->items[i] * v->items[i];
    return res;
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

