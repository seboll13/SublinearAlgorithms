#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "libs.h"
#include "helpers.h"
#include <arm_neon.h>
#include <pthread.h>

#define MAX_VEC_CAPACITY 1e9

typedef struct Vector {
    int capacity;
    float _Complex *items;
    char *name;
} Vector;

// Data structure to be used for multithreaded vector computations
typedef struct ThreadData {
    float _Complex *u_items;
    float _Complex *v_items;
    int length;
    float _Complex result;
} ThreadData;


// Vector type construction
void init_vector(Vector *v, char *name, int rows);
void free_vector(Vector *v);
void update_vector(Vector *v, float _Complex n, int idx);

// Vector population
Vector *rademacher_vector(int rows);

// Vector basic operations
Vector *vector_add(Vector *u, Vector *v, bool add);
Vector *vector_scalar_mult(Vector *u, int a);


float _Complex vector_dot_product(Vector *u, Vector *v);
float _Complex vector_dot_product_optimised(Vector *u, Vector *v);
void* thread_dot_product(void *arg);
float _Complex vector_dot_product_multithreaded(Vector *u, Vector *v, int num_threads);


Vector *vector_product(Vector *u, Vector *v);
float scalar_projection(Vector *u, Vector *v);
Vector *vector_projection(Vector *u, Vector *v);
float vector_angle_between(Vector *u, Vector *v, bool radians);

// Helper functions
bool check_vector_orthogonality(Vector *u, Vector *v);
bool check_vector_collinearity(Vector *u, Vector *v);
bool check_vector_perpendicularity(Vector *u, Vector *v);
bool check_vector_equality(Vector *u, Vector *v);
bool check_vector_oppositeness(Vector *u, Vector *v);
bool vector_is_integral(Vector *v);
bool vector_is_real(Vector *v);

// Norms
float vector_L1_norm(Vector *u);
float vector_L2_norm(Vector *u);
float vector_Lp_norm(Vector *u, int p);

// Vector str handling
void print_vector(Vector *v);
void print_integer_vector(Vector *v);
void print_real_vector(Vector *v);
void print_complex_vector(Vector *v);

#endif