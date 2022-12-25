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
    assert(v->items != NULL);
    if (v != NULL)
        free(v->items);
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

Vector *rademacher_vector(int rows) {
    assert(rows > 0);

    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", rows);

    for (int i = 0; i < rows; i++)
        update_vector(v, (rand() % 2 == 0) ? 1.0f + 0.0f * I : -1.0f + 0.0f * I, i);
    return v;
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
Vector *vector_scalar_mult(Vector *u, int a) {
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

    return (float) dot_product(u, v) / vector_L2_norm(v);
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
        update_vector(w, factor * v->items[i] / vector_L2_norm(v), i);
    return w;
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

    double res = dot_product(u, v) / (vector_L2_norm(u) * vector_L2_norm(v));

    assert(res >= -1 && res <= 1);
    return (radians) ? (float) acos(res) : radians_to_degrees(acos(res));
}

/**
 * @brief check whether or not two vectors are orthogonal
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are orthogonal
 * @return false otherwise (the angle between u and v is not 90deg)
 */
bool check_vector_orthogonality(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return dot_product(u, v) == 0;
}

/**
 * @brief check whether or not two vectors are collinear
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are collinear
 * @return false otherwise (the angle between u and v is not 0deg or 180deg)
 */
bool check_vector_collinearity(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    // one could also check that the vector product between u and v is 0
    return dot_product(u, v) == vector_L2_norm(u) * vector_L2_norm(v) || dot_product(u, v) == -vector_L2_norm(u) * vector_L2_norm(v);
}

/**
 * @brief check whether or not two vectors are perpendicular
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are perpendicular
 * @return false otherwise (the angle between u and v is not 90deg)
 */
bool check_vector_perpendicularity(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return dot_product(u, v) == 0;
}

/**
 * @brief check whether or not two vectors are equal
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are equal
 * @return false otherwise
 */
bool check_vector_equality(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    for (int i = 0; i < u->capacity; i++)
        if (u->items[i] != v->items[i])
            return false;
    return true;
}

/**
 * @brief check whether or not two vectors are opposite
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are opposite
 * @return false otherwise
 */
bool check_vector_oppositeness(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    for (int i = 0; i < u->capacity; i++)
        if (u->items[i] != -v->items[i])
            return false;
    return true;
}

/**
 * @brief check if a vector contains only integers
 * 
 * @param v vector
 * @return true if no element in v is a float
 * @return false otherwise
 */
bool vector_is_integral(Vector *v) {
    assert(v->capacity > 0);
    for (int i = 0; i < v->capacity; i++)
        if (v->items[i] != (int) v->items[i])
            return false;
    return true;
}

/**
 * @brief check if a vector contains only floats
 * 
 * @param v vector
 * @return true if no element in v is complex
 * @return false otherwise
 */
bool vector_is_real(Vector *v) {
    assert(v->capacity > 0);
    for (int i = 0; i < v->capacity; i++)
        if (cimag(v->items[i]) != 0)
            return false;
    return true;
}

/**
 * @brief compute the L1 norm of a vector
 * 
 * @param v vector
 * @return int sum of absolute values of each element of v
 */
float vector_L1_norm(Vector *v) {
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
float vector_L2_norm(Vector *v) {
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
float vector_Lp_norm(Vector *v, int p) {
    assert(p > 0 && v->capacity > 0);

    if (p == 1)
        return vector_L1_norm(v);
    if (p == 2)
        return vector_L2_norm(v);
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
    if (vector_is_integral(v))
        print_integer_vector(v);
    else if (vector_is_real(v))
        print_real_vector(v);
    else
        print_complex_vector(v);
}

/**
 * @brief print a vector of integers
 * 
 * @param v 
 */
void print_integer_vector(Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++)
        printf("     %d\n", (int) v->items[i]);
    printf(")\n");
}

/**
 * @brief print a vector of floats
 * 
 * @param v 
 */
void print_real_vector(Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++)
        printf("     %.3f\n", (float) v->items[i]);
    printf(")\n");
}

/**
 * @brief print a vector of complex numbers
 * 
 * @param v 
 */
void print_complex_vector(Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++) {
        float re = creal(v->items[i]); float im = cimag(v->items[i]);
        char sign = (im < 0.0f) ? '-' : '+';
        printf("     %.3f %c %.3fi\n", re, sign, fabs(im));
    }
    printf(")\n");
}