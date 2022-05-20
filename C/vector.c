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
    v->items = malloc(rows * sizeof(float _Complex));
    v->name = name;

    // Set the vector to all zeros by default
    for (int i = 0; i < rows; i++)
        v->items[i] = 0.0f + 0.0f * I;
}

/**
 * @brief Remove vector
 * 
 * @param v vector to be removed
 */
void free_vector(Vector *v) {
    if (v != NULL) {
        v->capacity = 0;
        v->name = NULL;
        free(v->items);
        free(v);
    }
    return;
}

/**
 * @brief Update a vector element
 * 
 * @param v vector to update
 * @param n element to add
 * @param idx position at which n is to be added
 */
void update_vector(Vector *v, float _Complex n, int idx) {
    assert(idx > -1);
    assert(idx < MAX_VEC_CAPACITY);
    
    v->items[idx] = n;
}

/**
 * @brief Computes the sum of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @param add subtract the two vectors if true
 * @return Vector* u+v (or u-v)
 */
Vector *vector_add(Vector *u, Vector *v, bool add) {
    assert(u->capacity == v->capacity);

    Vector *w = malloc(sizeof(Vector));
    init_vector(w, "W", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        if (add)
            update_vector(w, u->items[i] + v->items[i], i);
        else
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
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(v, a * u->items[i], i);
    return v;
}

/**
 * @brief Returns the scalar product of two vectors
 * Note: (a+ib)(c+id)=ac-bd+(ad+bc)i
 * 
 * @param u vector 1
 * @param v vector 2
 * @return int sum of products of each coefficient
 */
int scalar_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    float _Complex res = 0;
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
Vector *vector_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    Vector *w = malloc(sizeof(Vector));
    int vec_size = u->capacity;
    init_vector(w, "W", vec_size);

    for (int i = 0; i < vec_size; i++) {
        int u_index = (vec_size-2+i) % vec_size;
        int v_index = (u_index + 1) % vec_size;
        // obtain each element by circular permutation
        update_vector(w, u->items[u_index]*v->items[v_index]-u->items[v_index]*v->items[u_index], i);
    }
    return w;
}

float complex_abs(float _Complex z) {
    return sqrt(pow(creal(z),2) + pow(cimag(z),2));
}

/**
 * @brief Computes the L1 norm of a vector
 * 
 * @param u vector
 * @return int sum of absolute values of each element of u
 */
float L1_norm(Vector *u) {
    float res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += complex_abs(u->items[i]);
    return res;
}

/**
 * @brief Computes the L2 norm of a vector
 * 
 * @param u vector
 * @return double square root of the sum of squares
 */
float L2_norm(Vector *u) {
    float res = 0;
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
float Lp_norm(Vector *u, int p) {
    if (p == 1)
        return L1_norm(u);
    if (p == 2)
        return L2_norm(u);
    float res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += pow(u->items[i], (float) p);
    return pow(res, 1/p);
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
        float re = creal(v->items[i]); float im = cimag(v->items[i]);
        char sign = (im < 0.0f) ? '-' : '+';
        if (i == v->capacity - 1)
            printf("%.3f %c %.3fi", re, sign, fabs(im));
        else
            printf("%.3f %c %.3fi, ", re, sign, fabs(im));
    }
    printf(")\n");
}

