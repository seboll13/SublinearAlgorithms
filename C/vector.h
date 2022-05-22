#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "libs.h"

#define MAX_VEC_CAPACITY 10e3

typedef struct Vector {
    int capacity;
    float _Complex *items;
    char *name;
} Vector;

// Vector type construction
void init_vector(Vector *v, char *name, int rows);
void free_vector(Vector *v);
void update_vector(Vector *v, float _Complex n, int idx);

// Vector basic operations
Vector *vector_add(Vector *u, Vector *v, bool add);
Vector *vector_scalar_mult(Vector *u, int a);
float _Complex dot_product(Vector *u, Vector *v);
Vector *vector_product(Vector *u, Vector *v);
float scalar_projection(Vector *u, Vector *v);
Vector *vector_projection(Vector *u, Vector *v);

// Helper functions
float radians_to_degrees(float radians);
float vector_angle_between(Vector *u, Vector *v, bool radians);
float complex_abs(float _Complex z);
bool check_orthogonality(Vector *u, Vector *v);

// Norms
float L1_norm(Vector *u);
float L2_norm(Vector *u);
float Lp_norm(Vector *u, int p);

// Vector str handling
void print_vector(Vector *v);

#endif