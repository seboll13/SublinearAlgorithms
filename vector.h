#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "libs.h"

#define MAX_VEC_CAPACITY 10000

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
Vector *scalar_mult(Vector *u, int a);
int scalar_product(Vector *u, Vector *v);
Vector *vector_product(Vector *u, Vector *v);

// Helper functions
float complex_abs(float _Complex z);

// Norms
float L1_norm(Vector *u);
float L2_norm(Vector *u);
float Lp_norm(Vector *u, int p);

// Vector str handling
void print_vector(Vector *v);

#endif