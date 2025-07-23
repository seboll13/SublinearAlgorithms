#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "libs.h"
#include "helpers.h"
#include <string.h>
#include <errno.h>
// The current optimisations are for ARM processors only
// Future work will include Intel and AMD type processors
#ifdef __ARM_NEON
    #include <arm_neon.h>
    #define SIMD_ALIGNMENT 32
#else
    #define SIMD_ALIGNMENT 16
#endif

enum {
    VECTOR_SUCCESS = 0,
    VECTOR_ERR_BAD_SIZE = -1,
    VECTOR_ERR_OOM = -2
};

#define MAX_VEC_CAPACITY 1e9

// Vector data structure
typedef struct Vector {
    int capacity;
    float _Complex *items;
    char *name;
} Vector;


// ############################ VECTOR TYPE CONSTRUCTION ###############################
/**
 * @brief initialise a vector
 * 
 * @param v vector to initialise
 * @param name vector id
 * @param rows number of rows the vector will have
 * @return int status of the initialization (0 for success, negative int for failure)
 */
int init_vector(Vector *v, const char *name, int rows);

/**
 * @brief remove a vector from memory
 * 
 * @param v vector to be removed
 */
void free_vector(Vector *v);

/**
 * @brief update a vector element
 * 
 * Due to the high number of calls to this function, it is declared as inline, 
 * so that each GCC call to it will be replaced by the function's body at compile time
 * for faster execution.
 * 
 * @param v vector to update
 * @param n element to add
 * @param idx position at which n is to be added
 */
static inline void update_vector(Vector *v, float _Complex n, int idx) {
    v->items[idx] = n;
}
// ############################## VECTOR SAFETY CHECKS #################################

/**
 * @brief check whether or not both vectors contain at least one element
 * 
 * @param u first vector
 * @param v second vector
 * @return int 0 if both vectors contain at least one element, -1 otherwise (see constants above)
 */
int check_strictly_positive_sizes(const Vector *u, const Vector *v);
/**
 * @brief check whether or not two vectors have the same size
 * 
 * @param u first vector
 * @param v second vector
 * @return int 0 if the vectors have the same size, -1 otherwise (see constants above)
 */
int check_vector_sizes(const Vector *u, const Vector *v);

// ############################### VECTOR OPERATIONS ###################################

/**
 * @brief compute the sum of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @param add subtract the two vectors if true
 * @return Vector* u+v (or u-v)
 */
Vector *vector_add(Vector *u, Vector *v, bool add);

/**
 * @brief compute the multiplication of a vector by a scalar
 * 
 * @param u vector
 * @param a scalar
 * @return Vector* scaled vector
 */
Vector *vector_scalar_mult(Vector *u, int a);

/**
 * @brief get the dot product of two vectors using SIMD instructions
 *      and loop unrolling
 * Note: (a+ib)(c+id)=ac-bd+(ad+bc)i
 * 
 * @param u vector 1
 * @param v vector 2
 * @return int sum of products of each coefficient
 */
float _Complex vector_dot_product(const Vector *restrict u, const Vector *restrict v);

/**
 * @brief function to be executed by each thread when computing the dot product
 * 
 * @param arg 
 * @return void* pointer to the result of the computation
 */
//void* thread_dot_product(void *arg);

/**
 * @brief compute the dot product of two vectors using multiple threads
 * 
 * @param u vector 1
 * @param v vector 2
 * @param num_threads number of threads to use
 * @return float _Complex the dot product of the two vectors
 */
//float _Complex vector_dot_product_multithreaded(Vector *u, Vector *v, int num_threads);


/**
 * @brief compute the vector product of two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @return Vector result of vec prod
 */
Vector *vector_product(Vector *u, Vector *v);

/**
 * @brief get the scalar projection of a vector onto another one
 * 
 * @param u vector to project
 * @param v vector to project on
 * @return float scalar projection factor
 */
float scalar_projection(Vector *u, Vector *v);

/**
 * @brief get the vector projection of a vector onto another one
 * 
 * @param u vector to project
 * @param v vector to project on
 * @return Vector* vector projection
 */
Vector *vector_projection(Vector *u, Vector *v);

/**
 * @brief compute the angle between two vectors
 * 
 * @param u vector 1
 * @param v vector 2
 * @param radians return the angle in radians if true
 * @return float angle between the two vectors
 */
float vector_angle_between(Vector *u, Vector *v, bool radians);

// ############################### VECTOR GENERATION ###################################

/**
 * @brief construct a vector with Rademacher random variables, i.e. r.v. that are
 * either -1 or 1 with equal probability
 * 
 * @param rows 
 * @return Vector* pointer to the vector
 */
Vector *rademacher_vector(int rows);

// ############################### HELPER FUNCTIONS ####################################

/**
 * @brief check whether or not two vectors are orthogonal
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are orthogonal
 * @return false otherwise (the angle between u and v is not 90deg)
 */
bool check_vector_orthogonality(Vector *u, Vector *v);

/**
 * @brief check whether or not two vectors are collinear
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are collinear
 * @return false otherwise
 */
bool check_vector_collinearity(Vector *u, Vector *v);

/**
 * @brief check whether or not two vectors are perpendicular
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are perpendicular
 * @return false otherwise
 */
bool check_vector_perpendicularity(Vector *u, Vector *v);

/**
 * @brief check whether or not two vectors are equal
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v's elements are all one-to-one equal
 * @return false otherwise
 */
bool check_vector_equality(Vector *u, Vector *v);

/**
 * @brief check whether or not two vectors are opposite
 * 
 * @param u vector 1
 * @param v vector 2
 * @return true if u and v are opposite
 * @return false otherwise
 */
bool check_vector_oppositeness(Vector *u, Vector *v);

/**
 * @brief check whether or not a vector is integral
 * 
 * @param v vector to check
 * @return true if v is integral
 * @return false otherwise
 */
bool vector_is_integral(Vector *v);

/**
 * @brief check whether or not a vector is real
 * 
 * @param v vector to check
 * @return true if v is real
 * @return false otherwise
 */
bool vector_is_real(Vector *v);

// ############################### VECTOR NORMS ########################################

/**
 * @brief compute the L1 norm of a vector
 * 
 * @param v vector
 * @return int sum of absolute values of each element of v
 */
float vector_L1_norm(Vector *u);

/**
 * @brief compute the L2 norm of a vector
 * 
 * @param v vector
 * @return int square root of the sum of squares of each element of v
 */
float vector_L2_norm(Vector *u);

/**
 * @brief compute the Lp norm of a vector
 * 
 * @param v vector
 * @param p norm to compute
 * @return int p-th root of the sum of p-th powers of each element of v
 */
float vector_Lp_norm(Vector *u, int p);

// ############################### VECTOR PRINTING #####################################

/**
 * @brief general print function for vectors
 * 
 * @param v vector to be printed
 */
void print_vector(Vector *v);

/**
 * @brief print function to call in case of integer vectors
 * 
 * @param v vector to be printed
 */
void print_integer_vector(Vector *v);

/**
 * @brief print function to call when vector elements are real
 * 
 * @param v vector to be printed
 */
void print_real_vector(Vector *v);

/**
 * @brief print function to call when vector elements are complex
 * 
 * @param v vector to be printed
 */
void print_complex_vector(Vector *v);

#endif