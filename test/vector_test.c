#include <check.h>
#include "../C/vector.h"

/**
 * @brief Create a real 3d vector object
 * 
 * @param x first coordinate
 * @param y second coordinate
 * @param z third coordinate
 * @return Vector* constructed vector object
 */
Vector *create_real_3d_vector(float x, float y, float z) {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", 3);
    update_vector(v, x, 0); update_vector(v, y, 1); update_vector(v, z, 2);
    return v;
}

/**
 * @brief Create a dummy real vector object
 * 
 * @return Vector* constructed equal coordinate vector
 */
Vector *create_dummy_real_vector(float n) {
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", 3);
    for (int i = 0; i < 3; i++)
        update_vector(v, n, i);
    return v;
}

START_TEST(test_empty_vector_is_created_correctly)
{
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", 3);
    ck_assert_int_eq(v->capacity, 3);
    ck_assert_str_eq(v->name, "V");
    for (int i = 0; i < 3; i++) {
        ck_assert_float_eq(creal(v->items[i]), 0.0f);
        ck_assert_float_eq(cimag(v->items[i]), 0.0f);
    }
    free_vector(v);
    free(v);
}
END_TEST

START_TEST(test_new_vector_is_updated_correctly)
{
    Vector *v = create_dummy_real_vector(1.0f);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(v->items[i], 1.0f);
    free_vector(v);
    free(v);
}
END_TEST

START_TEST(test_standard_vector_addition)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    
    Vector *w = vector_add(u, v, true);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(w->items[i], 2.0f);
    free_vector(u); free_vector(v); free_vector(w);
    free(u); free(v); free(w);
}
END_TEST

START_TEST(test_standard_vector_subtraction)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    
    Vector *w = vector_add(u, v, false);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(w->items[i], 0.0f);
    free_vector(u); free_vector(v); free_vector(w);
    free(u); free(v); free(w);
}
END_TEST

START_TEST(test_standard_scalar_multiplication)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = vector_scalar_mult(u, 2);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(v->items[i], 2.0f);
    free_vector(v);
    free(v);
}
END_TEST

START_TEST(test_standard_dot_product)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    int dot_prod = dot_product(u, v);
    ck_assert_float_eq(dot_prod, 3.0f);
    free_vector(u); free_vector(v);
    free(u); free(v);
}
END_TEST

START_TEST(test_standard_vector_product)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    
    Vector *w = vector_product(u, v);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(w->items[i], 0.0f);
    free_vector(u); free_vector(v); free_vector(w);
    free(u); free(v); free(w);
}
END_TEST

START_TEST(test_standard_scalar_projection)
{
    Vector *u = create_real_3d_vector(1, 0, 0);
    Vector *v = create_real_3d_vector(1, 1, 0);

    ck_assert_float_eq(1 / sqrt(2), scalar_projection(u, v));
    free_vector(u); free_vector(v);
    free(u); free(v);
}
END_TEST

START_TEST(test_standard_vector_projection)
{
    Vector *u = create_real_3d_vector(1, 0, 0);
    Vector *v = create_real_3d_vector(1, 1, 0);
    Vector *w = vector_projection(u, v);
    ck_assert_float_eq(0.5f, w->items[0]);
    ck_assert_float_eq(0.5f, w->items[1]);
    ck_assert_float_eq(0.0f, w->items[2]);
    free_vector(u); free_vector(v); free_vector(w);
    free(u); free(v); free(w);
}
END_TEST

START_TEST(test_standard_L1_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = L1_norm(u);
    ck_assert_float_eq(n, 6.0f);
    free_vector(u);
    free(u);
}
END_TEST

START_TEST(test_standard_L2_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = L2_norm(u);
    ck_assert_float_eq(n, sqrt(12));
    free_vector(u);
    free(u);
}
END_TEST

START_TEST(test_first_level_Lp_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = Lp_norm(u, 1);
    ck_assert_float_eq(n, 6.0f);
    free_vector(u);
    free(u);
}
END_TEST

START_TEST(test_second_level_Lp_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = Lp_norm(u, 2);
    ck_assert_float_eq(n, sqrt(12));
    free_vector(u);
    free(u);
}
END_TEST

START_TEST(test_third_level_Lp_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = Lp_norm(u, 3);
    ck_assert_float_eq(n, pow(24, 1/3));
    free_vector(u);
    free(u);
}
END_TEST

START_TEST(test_correct_complex_absolute_value) 
{
    float _Complex z = 2.0f + 1.0f * I;
    float cabs = complex_abs(z);
    ck_assert_float_eq(cabs, sqrt(pow(2,2)+pow(1,2)));
}
END_TEST

START_TEST(test_orthogonality_for_two_orthogonal_vectors)
{
    Vector *u = create_real_3d_vector(1, -1, 0);
    Vector *v = create_real_3d_vector(1, 1, -1);
    ck_assert(check_orthogonality(u,v));
    free_vector(u); free_vector(v);
    free(u); free(v);
}
END_TEST

START_TEST(test_orthogonality_for_two_non_orthogonal_vectors)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    ck_assert(!check_orthogonality(u,v));
    free_vector(u); free_vector(v);
    free(u); free(v);
}
END_TEST

START_TEST(test_standard_radians_to_degrees)
{
    ck_assert_float_eq(180.0f, radians_to_degrees(M_PI));
    ck_assert_float_eq(180.0f, radians_to_degrees(3*M_PI));
}
END_TEST

START_TEST(test_correct_angle_between_vector)
{
    Vector *u = create_real_3d_vector(1, -1, 0);
    Vector *v = create_real_3d_vector(1, 1, -1);
    ck_assert_float_eq(90.0f, vector_angle_between(u, v, false));
    ck_assert_float_eq(M_PI_2, vector_angle_between(u, v, true));
    free_vector(u); free_vector(v);
    free(u); free(v);
}
END_TEST

START_TEST(test_orthogonality_yields_right_angle)
{
    Vector *u = malloc(sizeof(Vector));
    Vector *v = malloc(sizeof(Vector));
    init_vector(u, "U", 3);
    init_vector(v, "V", 3);
    u->items[0] = 1; u->items[1] = -1; u->items[2] = 0;
    v->items[0] = 1; v->items[1] = 1; v->items[2] = -1;
    ck_assert_float_eq(90.0f, vector_angle_between(u, v, false));
    free_vector(u); free_vector(v);
    free(u); free(v);
}
END_TEST