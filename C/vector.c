#include "vector.h"

/**
 * @brief initialise a vector
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
 * @brief remove a vector
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
 * @brief update a vector element
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
 * @brief compute the sum of two vectors
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
 * @brief compute the multiplication of a vector by a scalar
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
 * @brief get the dot product of two vectors
 * Note: (a+ib)(c+id)=ac-bd+(ad+bc)i
 * 
 * @param u vector 1
 * @param v vector 2
 * @return int sum of products of each coefficient
 */
float _Complex dot_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    float _Complex res = 0;
    for (int i = 0; i < u->capacity; i++)
        res += u->items[i] * v->items[i];
    return res;
}

/**
 * @brief return the vector product of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return Vector result of vec prod
 */
Vector *vector_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    int vec_size = u->capacity;
    Vector *w = malloc(sizeof(Vector));
    init_vector(w, "W", vec_size);

    for (int i = 0; i < vec_size; i++) {
        int u_index = (vec_size-2+i) % vec_size;
        int v_index = (u_index + 1) % vec_size;
        // obtain each element by circular permutation
        update_vector(w, u->items[u_index]*v->items[v_index]-u->items[v_index]*v->items[u_index], i);
    }
    return w;
}

/**
 * @brief return the scalar projection of a vector onto another one
 * 
 * @param u vector to project
 * @param v vector to project on
 * @return float scalar projection factor
 */
float scalar_projection(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return (float) dot_product(u, v) / L2_norm(v);
}

/**
 * @brief return the projection of a vector onto another one
 * 
 * @param u vector to project
 * @param v vector to project on
 * @return Vector* the projection vector of u onto v
 */
Vector *vector_projection(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    float factor = scalar_projection(u, v);

    Vector *w = malloc(sizeof(Vector));
    init_vector(w, "W", u->capacity);
    for (int i = 0; i < w->capacity; i++)
        update_vector(w, factor * v->items[i] / L2_norm(v), i);
    return w;
}

/**
 * @brief helper to convert radians to degrees
 * 
 * @param radians 
 * @return float 
 */
float radians_to_degrees(float radians) {
    return (float) 180 * fmod(radians, 2*M_PI) / M_PI;
}

/**
 * @brief return the angle between two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @param radians boolean to indicate the result format
 * @return float angle between u and v in radians if radians is true, in degrees otherwise
 */
float vector_angle_between(Vector *u, Vector *v, bool radians) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    double res = dot_product(u, v) / (L2_norm(u) * L2_norm(v));

    assert(res >= -1 && res <= 1);
    return (radians) ? (float) acos(res) : radians_to_degrees(acos(res));
}

/**
 * @brief return the complex absolute value of some complex number
 * 
 * @param z complex number
 * @return float L2 distance on the complex plane (i.e. norm of a complex)
 */
float complex_abs(float _Complex z) {
    return sqrt(pow(creal(z),2) + pow(cimag(z),2));
}

/**
 * @brief check whether or not two vectors are orthogonal
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are orthogonal
 * @return false otherwise (the angle between u and v is not 90deg)
 */
bool check_orthogonality(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return dot_product(u, v) == 0;
}

/**
 * @brief compute the L1 norm of a vector
 * 
 * @param v vector
 * @return int sum of absolute values of each element of v
 */
float L1_norm(Vector *v) {
    assert(v->capacity > 0);

    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += complex_abs(v->items[i]);
    return res;
}

/**
 * @brief compute the L2 norm of a vector
 * 
 * @param v vector
 * @return double square root of the sum of squares
 */
float L2_norm(Vector *v) {
    assert(v->capacity > 0);

    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += pow(v->items[i], 2.0);
    return sqrt(res); 
}

/**
 * @brief compute the Lp norm of a vector
 * 
 * @param v vector
 * @param p power parameter
 * @return double p'th root of the sum of p-powers
 */
float Lp_norm(Vector *v, int p) {
    assert(p > 0 && v->capacity > 0);

    if (p == 1)
        return L1_norm(v);
    if (p == 2)
        return L2_norm(v);
    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += pow(v->items[i], (float) p);
    return pow(res, 1/p);
}

/**
 * @brief print vector elements to screen
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