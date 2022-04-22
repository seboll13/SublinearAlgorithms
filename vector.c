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
Vector vector_add(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    Vector w;
    init_vector(&w, "W", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(&w, u->items[i] + v->items[i], i);
    return w;
}

/**
 * @brief Computes the difference between two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return Vector* u-v
 */
Vector vector_sub(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    Vector w;
    init_vector(&w, "W", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(&w, u->items[i] - v->items[i], i);
    return w;
}

/**
 * @brief Computes the multiplication of a vector by a scalar
 * 
 * @param u vector
 * @param a scalar
 * @return Vector* scaled vector
 */
Vector scalar_mult(Vector *u, int a) {
    Vector v;
    init_vector(&v, "V", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(&v, a * u->items[i], i);
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
 * @brief Returns the vector product of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return Vector result of vec prod
 */
Vector vector_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    Vector w;
    int vec_size = u->capacity;
    init_vector(&w, "W", vec_size);

    for (int i = 0; i < vec_size; i++) {
        int u_index = (vec_size-2+i) % vec_size;
        int v_index = (u_index + 1) % vec_size;
        // obtain each element by circular permutation
        update_vector(&w, u->items[u_index]*v->items[v_index]-u->items[v_index]*v->items[u_index], i);
    }
    return w;
}

/**
 * @brief Computes the L1 norm of a vector
 * 
 * @param u vector
 * @return int sum of absolute values of each element of u
 */
int L1_norm(Vector *u) {
    int res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += abs(u->items[i]);
    return res;
}

/**
 * @brief Computes the L2 norm of a vector
 * 
 * @param u vector
 * @return double square root of the sum of squares
 */
double L2_norm(Vector *u) {
    double res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += pow(u->items[i], 2.0);
    return sqrt(res); 
}

/**
 * @brief Computes the Lp norm of a vector
 * 
 * @param u vector
 * @param p power parameter
 * @return double p'th root of the sum of p-powers
 */
double Lp_norm(Vector *u, int p) {
    if (p == 1)
        return L1_norm(u);
    if (p == 2)
        return L2_norm(u);
    double res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += pow(u->items[i], (double) p);
    return pow(res, 1/p);
}

/**
 * @brief Computes the L-infinity norm of a vector
 * 
 * @param u vector
 * @return int maximum absolute value in u
 */
int Linf_norm(Vector *u) {
    int res = abs(u->items[0]);
    for (int i = 1; i < u->capacity; i++)
        if (abs(u->items[i]) > res)
            res = abs(u->items[i]);
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

