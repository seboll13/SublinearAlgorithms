#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "libs.h"

#define MAX_VEC_CAPACITY 10000

typedef struct Vector {
    int capacity;
    int *items;
    char *name;
} Vector;

// Vector type construction
void init_vector(Vector *v, char *name, int rows);
void free_vector(Vector *v);
void update_vector(Vector *v, int n, int idx);

// Vector basic operations
Vector *vector_add(Vector *u, Vector *v);
Vector *vector_sub(Vector *u, Vector *v);
Vector *scalar_mult(Vector *u, int a);
int scalar_product(Vector *u, Vector *v);
Vector *vector_product(Vector *u, Vector *v);

// Helper functions
// ...

// Norms
int L1_norm(Vector *u);
double L2_norm(Vector *u);
double Lp_norm(Vector *u, int p);
int Linf_norm(Vector *u);

// Vector str handling
void print_vector(Vector *v);

#endif