#include "vector.h"

// ############################ VECTOR TYPE CONSTRUCTION ###############################

void init_vector(Vector *v, char *name, int rows) {
    assert(rows > 0);

    v->capacity = rows;
    v->name = name;
    //v->items = malloc(rows * sizeof(float _Complex));
    float _Complex *items;
    int ret = posix_memalign((void**)&items, 32, rows * sizeof(float _Complex));
    if (ret != 0) {
        items = NULL;
        perror("posix_memalign failed");
        exit(EXIT_FAILURE);
    }
    v->items = items;

    // Set the vector to all zeros by default
    for (int i = 0; i < rows; i++)
        v->items[i] = 0.0f + 0.0f * I;
}

void free_vector(Vector *v) {
    assert(v->items != NULL);
    if (v != NULL)
        free(v->items);
    return;
}

void update_vector(Vector *v, float _Complex n, int idx) {
    assert(idx > -1);
    assert(idx < MAX_VEC_CAPACITY);
    
    v->items[idx] = n;
}

// ############################### VECTOR OPERATIONS ###################################

Vector *rademacher_vector(int rows) {
    assert(rows > 0);

    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", rows);

    for (int i = 0; i < rows; i++)
        update_vector(v, (rand() % 2 == 0) ? 1.0f + 0.0f * I : -1.0f + 0.0f * I, i);
    return v;
}

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

Vector *vector_scalar_mult(Vector *u, int a) {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(v, a * u->items[i], i);
    return v;
}

float _Complex vector_dot_product(Vector *u, Vector *v) {
    assert(u->capacity == v->capacity);

    int i = 0;
    const int simd_width = 4; // Number of float pairs processed per NEON operation
    const int unroll_factor = 4; // Unroll the loop to process more elements at once
    float32x4_t real_sum = vdupq_n_f32(0.0);
    float32x4_t imag_sum = vdupq_n_f32(0.0);

    for (; i <= u->capacity - unroll_factor * simd_width; i += unroll_factor * simd_width) {
        for (int j = 0; j < unroll_factor; j++) {
            float32x4x2_t u_vec = vld2q_f32((float*)&u->items[i + j * simd_width]);
            float32x4x2_t v_vec = vld2q_f32((float*)&v->items[i + j * simd_width]);

            float32x4_t real_part = vmlsq_f32(vmulq_f32(u_vec.val[0], v_vec.val[0]), u_vec.val[1], v_vec.val[1]);
            float32x4_t imag_part = vmlaq_f32(vmulq_f32(u_vec.val[0], v_vec.val[1]), u_vec.val[1], v_vec.val[0]);

            real_sum = vaddq_f32(real_sum, real_part);
            imag_sum = vaddq_f32(imag_sum, imag_part);
        }
    }

    // Combine the sums from SIMD operations
    float real_result = vaddvq_f32(real_sum);
    float imag_result = vaddvq_f32(imag_sum);

    // Handle remaining elements that were not processed by the unrolled loop
    for (; i < u->capacity; i++) {
        real_result += crealf(u->items[i]) * crealf(v->items[i]) - cimagf(u->items[i]) * cimagf(v->items[i]);
        imag_result += crealf(u->items[i]) * cimagf(v->items[i]) + cimagf(u->items[i]) * crealf(v->items[i]);
    }

    return real_result + imag_result * I;
}

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

float scalar_projection(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return (float) vector_dot_product(u, v) / vector_L2_norm(v);
}

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

float vector_angle_between(Vector *u, Vector *v, bool radians) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    double res = vector_dot_product(u, v) / (vector_L2_norm(u) * vector_L2_norm(v));

    assert(res >= -1 && res <= 1);
    return (radians) ? (float) acos(res) : radians_to_degrees(acos(res));
}

// ############################### HELPER FUNCTIONS ####################################

bool check_vector_orthogonality(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return vector_dot_product(u, v) == 0;
}

bool check_vector_collinearity(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    // one could also check that the vector product between u and v is 0
    return vector_dot_product(u, v) == vector_L2_norm(u) * vector_L2_norm(v) || vector_dot_product(u, v) == -vector_L2_norm(u) * vector_L2_norm(v);
}

bool check_vector_perpendicularity(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    return vector_dot_product(u, v) == 0;
}

bool check_vector_equality(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    for (int i = 0; i < u->capacity; i++)
        if (u->items[i] != v->items[i])
            return false;
    return true;
}

bool check_vector_oppositeness(Vector *u, Vector *v) {
    assert(u->capacity > 0 && v->capacity > 0);
    assert(u->capacity == v->capacity);

    for (int i = 0; i < u->capacity; i++)
        if (u->items[i] != -v->items[i])
            return false;
    return true;
}

bool vector_is_integral(Vector *v) {
    assert(v->capacity > 0);
    for (int i = 0; i < v->capacity; i++)
        if (v->items[i] != (int) v->items[i])
            return false;
    return true;
}

bool vector_is_real(Vector *v) {
    assert(v->capacity > 0);
    for (int i = 0; i < v->capacity; i++)
        if (cimag(v->items[i]) != 0)
            return false;
    return true;
}

// ############################### VECTOR NORMS ########################################

float vector_L1_norm(Vector *v) {
    assert(v->capacity > 0);

    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += complex_abs(v->items[i]);
    return res;
}

float vector_L2_norm(Vector *v) {
    assert(v->capacity > 0);

    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += pow(v->items[i], 2.0);
    return sqrt(res); 
}

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

// ############################### VECTOR PRINTING #####################################

void print_vector(Vector *v) {
    assert(v->capacity > 0);
    if (vector_is_integral(v))
        print_integer_vector(v);
    else if (vector_is_real(v))
        print_real_vector(v);
    else
        print_complex_vector(v);
}

void print_integer_vector(Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++)
        printf("     %d\n", (int) v->items[i]);
    printf(")\n");
}

void print_real_vector(Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++)
        printf("     %.3f\n", (float) v->items[i]);
    printf(")\n");
}

void print_complex_vector(Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++) {
        float re = creal(v->items[i]); float im = cimag(v->items[i]);
        char sign = (im < 0.0f) ? '-' : '+';
        printf("     %.3f %c %.3fi\n", re, sign, fabs(im));
    }
    printf(")\n");
}