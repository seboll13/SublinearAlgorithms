#include "projections.h"

/**
 * @brief generate a number between 0 and 1 uniformly at random
 * 
 * @return float probability value
 */
float generate_uniform_probability() {
    float p = (float) rand() / (float) RAND_MAX;
    assert(p >= 0 && p <=1);
    return p;
}

/**
 * @brief generate a random sign
 * 
 * @return int +1 or -1
 */
int sign() {
    return rand()%2 * 2 - 1;
}

/**
 * @brief generate a random complex number
 * 
 * @return float the resulting number
 */
float _Complex generate_random_complex() {
    float re = (float) generate_uniform_probability() * MAX_RND_COMPLEX * sign();
    float im = (float) generate_uniform_probability() * MAX_RND_COMPLEX * sign();
    return re + im * I;
}

/**
 * @brief generate a random complex vector (with coefficient values between -1 and 1)
 * 
 * @return Vector the resulting vector
 */
Vector *generate_random_vector(int dim) {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", dim);
    
    for (int i = 0; i < dim; i++)
        update_vector(v, generate_random_complex(), i);
    return v;
}

/**
 * @brief generate r.v.'s that follow a standard gaussian distribution using Box-Muller
 * 
 * @param num_variables # of variables
 * @return float* array of gaussians
 */
float *generate_gaussian_random_variables(int num_variables) {
    float *rvs = malloc(sizeof(float) * num_variables);
    for (int i = 0; i < num_variables; i++) {
        float U1 = generate_uniform_probability();
        float U2 = generate_uniform_probability();
        float Z = sqrt(-2*log(U1)) * cos(2*M_PI*U2);
        *(rvs + i) = Z; // equivalent to rvs[i] = Z;
    }
    return rvs;
}

/**
 * @brief generate a sample of coordinates such that the point lies on the unit sphere
 * 
 * @param dim # of dimensions
 * @return float* array of coordinates
 */
float *generate_sample_from_unit_sphere(int dim) {
    float *sample = generate_gaussian_random_variables(dim);

    float squares = 0.0f;
    
    while (*sample++ != '\0')
        squares += pow(*sample, 2);
    
    float norm = sqrt(squares);
    float sum = 0.0f;
    sample -= dim;
    while (*sample++ != '\0') {
        *sample /= norm;
        sum += *sample;
    }
    printf("%lf\n", sum);
    // assert(sum == 1);
    return sample;
}

/**
 * @brief create a random vector that lies on the unit sphere
 * 
 * @param dim # of dimensions
 * @return Vector* the resulting vector
 */
Vector *random_vector_on_unit_sphere(int dim) {
    Vector *a = malloc(sizeof(Vector));
    init_vector(a, "a", dim);

    float *random_sample = generate_sample_from_unit_sphere(dim);
    for (int i = 0; i < dim; i++)
        update_vector(a, *(random_sample + i), i);
    return a;
}

/**
 * @brief say whether or not the vector random projection lemma holds
 * 
 * @param m # of dimensions
 * @param delta random value between 0 and 1
 * @return bool 
 */
bool vector_random_projection(int m, float delta) {
    Vector *v = generate_random_vector(m);
    
    float v_norm = vector_L2_norm(v);
    float upper_bound = v_norm * delta / sqrt(exp(1) * m);
    
    int cnt = 0;
    int range = 1;
    while (range--) {
        Vector *a = random_vector_on_unit_sphere(m);
        float dot_prod = cabsf(vector_inner_product(a, v));
        if (dot_prod <= upper_bound)
            cnt += 1;
    }
    return (cnt / m) <= delta;
}