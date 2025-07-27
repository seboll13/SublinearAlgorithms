#include "vector.h"

// ############################ VECTOR TYPE CONSTRUCTION ###############################

__attribute__((cold))
int init_vector(Vector *v, const char *name, int rows) {
    if (rows <= 0) {
        fprintf(stderr, "init_vector: bad size %d\n", rows);
        errno = EINVAL;
        return VECTOR_ERR_BAD_SIZE;
    }

    v->name = strdup(name);
    if (!v->name) {
        perror("strdup failed");
        errno = ENOMEM;
        return VECTOR_ERR_OOM;
    }

    v->capacity = rows;
    
    float _Complex *items;

    int ret = posix_memalign((void **)&items, SIMD_ALIGNMENT, rows * sizeof *items);
    if (ret != 0) {
        items = NULL;
        perror("posix_memalign failed");
        free(v->name);
        errno = ENOMEM;
        return VECTOR_ERR_OOM;
    }

    v->items = __builtin_assume_aligned(items, SIMD_ALIGNMENT);
    memset(v->items, 0, (size_t) rows * sizeof *v->items);
    return VECTOR_SUCCESS;
}

void free_vector(Vector *v) {
    if (v->items != NULL)
        free(v->items);
    return;
}

// ############################## VECTOR SAFETY CHECKS #################################

int check_strictly_positive_sizes(const Vector *u, const Vector *v) {
    if (u->capacity < 1 || v->capacity < 1) {
        fprintf(stderr, "Vectors must contain at least one element\n");
        errno = EINVAL;
        return VECTOR_ERR_BAD_SIZE;
    }
    return VECTOR_SUCCESS;
}

int check_vector_sizes(const Vector *u, const Vector *v) {
    if (u->capacity != v->capacity) {
        fprintf(stderr, "Vectors must have the same size\n");
        errno = EINVAL;
        return VECTOR_ERR_BAD_SIZE;
    }
    return VECTOR_SUCCESS;
}

// ############################### VECTOR OPERATIONS ###################################

Vector *vector_add(const Vector *u, const Vector *v, bool add) {
    if (check_vector_sizes(u, v) != VECTOR_SUCCESS)
        return NULL;

    Vector *w = malloc(sizeof(Vector));
    init_vector(w, "W", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        if (add)
            update_vector(w, u->items[i] + v->items[i], i);
        else
            update_vector(w, u->items[i] - v->items[i], i);
    return w;
}

Vector *vector_scalar_mult(const Vector *u, int a) {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", u->capacity);

    for (int i = 0; i < u->capacity; i++)
        update_vector(v, (float _Complex) a * u->items[i], i);
    return v;
}

__attribute__((hot))
float _Complex vector_inner_product(const Vector *u, const Vector *v) {
    if (check_vector_sizes(u, v) != VECTOR_SUCCESS)
        return -1;

    float real_result = 0.0f;
    float imag_result = 0.0f;

    #ifdef __ARM_NEON
    int i = 0;
    const int simd_width = 4; // Number of float pairs processed per NEON operation
    const int unroll_factor = 4; // Unroll the loop to process more elements at once
    float32x4_t real_sum = vdupq_n_f32(0.0);
    float32x4_t imag_sum = vdupq_n_f32(0.0);

    bool is_v_real = vector_is_real(v);
    for (; i <= u->capacity - unroll_factor * simd_width; i += unroll_factor * simd_width) {
        for (int j = 0; j < unroll_factor; j++) {
            // Prefetch future elements to reduce cache misses
            __builtin_prefetch(&u->items[i + (j + 1) * simd_width], 0, 1);
            __builtin_prefetch(&v->items[i + (j + 1) * simd_width], 0, 1);

            float32x4x2_t u_vec = vld2q_f32((float*)&u->items[i + j * simd_width]);
            float32x4x2_t v_vec = vld2q_f32((float*)&v->items[i + j * simd_width]);

            // Complex vectors require the Hermitian dot product, i.e.
            // conjugating the imaginary part of v
            if (!is_v_real) v_vec.val[1] = vmulq_n_f32(v_vec.val[1], -1.0f);

            float32x4_t real_part = vmlsq_f32(vmulq_f32(u_vec.val[0], v_vec.val[0]), u_vec.val[1], v_vec.val[1]);
            float32x4_t imag_part = vmlaq_f32(vmulq_f32(u_vec.val[1], v_vec.val[0]), u_vec.val[0], v_vec.val[1]);

            real_sum = vaddq_f32(real_sum, real_part);
            imag_sum = vaddq_f32(imag_sum, imag_part);
        }
    }

    // Combine the sums from SIMD operations
    real_result = vaddvq_f32(real_sum);
    imag_result = vaddvq_f32(imag_sum);

    // Handle remaining elements that were not processed by the unrolled loop
    for (; i < u->capacity; i++) {
        float a = crealf(u->items[i]);
        float b = cimagf(u->items[i]);
        float c = crealf(v->items[i]);
        float d = cimagf(v->items[i]);

        if (!is_v_real) d = -d; // Conjugate the imaginary part if v is complex
        real_result += a * c - b * d;
        imag_result += a * d + b * c;
    }
    #else
    // We fallback to non-SIMD version for non-ARM architectures
    for (int i = 0; i < u->capacity; i++) {
        float a = crealf(u->items[i]);
        float b = cimagf(u->items[i]);
        float c = crealf(v->items[i]);
        float d = cimagf(v->items[i]);

        if (!is_v_real) d = -d; // Conjugate the imaginary part if v is complex
        real_result += a * c - b * d;
        imag_result += a * d + b * c;
    }
    #endif

    return real_result + imag_result * I;
}

Vector *vector_product(const Vector *u, const Vector *v) {
    if (check_vector_sizes(u, v) != VECTOR_SUCCESS)
        return NULL;

    int vec_size = u->capacity;
    int N = 1 << (int)ceil(log2(vec_size)); // get the next power of 2

    Vector *w = malloc(sizeof(Vector));
    init_vector(w, "W", vec_size);

    int u_index = vec_size - 2;
    int v_index = vec_size - 1;
    for (int i = 0; i < vec_size; i++) {
        // obtain each element by circular permutation
        update_vector(w, u->items[u_index] * v->items[v_index] - u->items[v_index] * v->items[u_index], i);

        // update indices using bitwise operations for better performance
        u_index = (u_index + 1) & (N - 1);
        v_index = (v_index + 1) & (N - 1);
    }
    return w;
}

float scalar_projection(const Vector *u, const Vector *v) {
    if (check_vector_sizes(u, v) != VECTOR_SUCCESS || check_strictly_positive_sizes(u, v) != VECTOR_SUCCESS || vector_L2_norm(v) == 0)
        return VECTOR_ERR_BAD_SIZE;

    return (float) vector_inner_product(u, v) / vector_L2_norm(v);
}

Vector *vector_projection(const Vector *u, const Vector *v) {
    if (check_vector_sizes(u, v) != VECTOR_SUCCESS || check_strictly_positive_sizes(u, v) != VECTOR_SUCCESS || vector_L2_norm(v) == 0)
        return NULL;

    float factor = scalar_projection(u, v);

    Vector *w = malloc(sizeof(Vector));
    init_vector(w, "W", u->capacity);
    
    float l2_norm = vector_L2_norm(v);
    for (int i = 0; i < u->capacity; i++)
        update_vector(w, factor * v->items[i] / l2_norm, i);
    return w;
}

float vector_angle_between(const Vector *u, const Vector *v, bool radians) {
    if (check_vector_sizes(u, v) != VECTOR_SUCCESS || check_strictly_positive_sizes(u, v) != VECTOR_SUCCESS || vector_L2_norm(u) == 0)
        return VECTOR_ERR_BAD_SIZE;

    float res = crealf(vector_inner_product(u, v)) / (vector_L2_norm(u) * vector_L2_norm(v));

    assert(res >= -1 && res <= 1);
    return radians ? acosf(res) : radians_to_degrees(acosf(res));
}

// ############################### VECTOR GENERATION ###################################

Vector *rademacher_vector(int rows) {
    assert(rows > 0);

    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", rows);

    for (int i = 0; i < rows; i++)
        update_vector(v, (rand() % 2 == 0) ? 1.0f + 0.0f * I : -1.0f + 0.0f * I, i);
    return v;
}

// ############################### HELPER FUNCTIONS ####################################

bool check_vector_orthogonality(const Vector *u, const Vector *v) {
    return vector_inner_product(u, v) == 0;
}

bool check_vector_collinearity(const Vector *u, const Vector *v) {
    // one could also check that the vector product between u and v is 0
    return vector_inner_product(u, v) == vector_L2_norm(u) * vector_L2_norm(v) || vector_inner_product(u, v) == -vector_L2_norm(u) * vector_L2_norm(v);
}

bool check_vector_perpendicularity(const Vector *u, const Vector *v) {
    return vector_inner_product(u, v) == 0;
}

bool check_vector_equality(const Vector *u, const Vector *v) {
    return memcmp(u->items, v->items,
                  u->capacity * sizeof *u->items) == 0;
}

bool check_vector_oppositeness(const Vector *u, const Vector *v) {
    for (int i = 0; i < u->capacity; i++)
        if (u->items[i] != -v->items[i])
            return false;
    return true;
}

bool vector_is_integral(const Vector *v) {
    assert(v->capacity > 0);
    for (int i = 0; i < v->capacity; i++) {
        float real = crealf(v->items[i]);
        float imag = cimagf(v->items[i]);
        if (fabs(imag) > 1e-6f || fabs(real - roundf(real)) > 1e-6f)
            return false;
    }
    return true;
}

bool vector_is_real(const Vector *v) {
    assert(v->capacity > 0);
    for (int i = 0; i < v->capacity; i++)
        if (cimagf(v->items[i]) != 0)
            return false;
    return true;
}

// ############################### VECTOR NORMS ########################################

float vector_L1_norm(const Vector *v) {
    if (v->capacity == 0)
        return -1;

    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += cabsf(v->items[i]);
    return res;
}

float vector_L2_norm(const Vector *v) {
    if (v->capacity == 0)
        return -1;

    float res = 0;
    for (int i = 0; i < v->capacity; i++) {
        float a = crealf(v->items[i]);
        float b = cimagf(v->items[i]);
        res += a * a + b * b;
    }
    return sqrtf(res);
}

float vector_Lp_norm(const Vector *v, int p) {
    assert(p > 0 && v->capacity > 0);

    if (p == 1)
        return vector_L1_norm(v);
    if (p == 2)
        return vector_L2_norm(v);
    float res = 0;
    for (int i = 0; i < v->capacity; i++)
        res += powf(cabsf(v->items[i]), (float) p);
    return powf(res, 1.0f / (float) p);
}

// ############################### VECTOR PRINTING #####################################

void print_vector(const Vector *v) {
    assert(v->capacity > 0);
    if (vector_is_integral(v))
        print_integer_vector(v);
    else if (vector_is_real(v))
        print_real_vector(v);
    else
        print_complex_vector(v);
}

void print_integer_vector(const Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++)
        printf("     %d\n", (int) v->items[i]);
    printf(")\n");
}

void print_real_vector(const Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++)
        printf("     %.3f\n", (float) v->items[i]);
    printf(")\n");
}

void print_complex_vector(const Vector *v) {
    printf("%s = (\n", v->name);
    for (int i = 0; i < v->capacity; i++) {
        float re = crealf(v->items[i]); float im = cimagf(v->items[i]);
        char sign = (im < 0.0f) ? '-' : '+';
        printf("     %.3f %c %.3fi\n", re, sign, fabsf(im));
    }
    printf(")\n");
}