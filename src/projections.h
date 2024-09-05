#ifndef PROJECTIONS_HEADER
#define PROJECTIONS_HEADER

#include "vector.h"

#define MAX_RND_COMPLEX 1

float _Complex generate_random_complex();
Vector *generate_random_vector(int dim);
float generate_uniform_probability();
float *generate_gaussian_random_variables(int num_variables);
float *generate_sample_from_unit_sphere(int dim);
Vector *random_vector_on_unit_sphere(int dim);
bool vector_random_projection(int m, float delta);
#endif