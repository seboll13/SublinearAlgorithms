#include <check.h>
#include "../C/vector.h"

/**
 * @brief Create a dummy real vector object
 * 
 * @return Vector* 
 */
Vector *create_dummy_real_vector(float n) {
    Vector *u = malloc(sizeof(Vector));
    init_vector(u, "U", 3);
    for (int i = 0; i < 3; i++)
        update_vector(u, n, i);
    return u;
}

START_TEST(test_empty_vector_is_created_correctly)
{
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", 3);
    for (int i = 0; i < 3; i++) {
        ck_assert_float_eq(creal(v->items[i]), 0.0f);
        ck_assert_float_eq(cimag(v->items[i]), 0.0f);
    }
    free_vector(v);
}
END_TEST

START_TEST(test_new_vector_is_updated_correctly)
{
    Vector *v = create_dummy_real_vector(1.0f);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(v->items[i], 1.0f);
    free_vector(v);
}
END_TEST

START_TEST(test_standard_vector_addition)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    
    Vector *w = malloc(sizeof(Vector));
    w = vector_add(u, v, true);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(w->items[i], 2.0f);
    free_vector(u); free_vector(v); free_vector(w);
}
END_TEST

START_TEST(test_standard_vector_subtraction)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    
    Vector *w = malloc(sizeof(Vector));
    w = vector_add(u, v, false);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(w->items[i], 0.0f);
    free_vector(u); free_vector(v); free_vector(w);
}
END_TEST

START_TEST(test_standard_scalar_multiplication)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = scalar_mult(u, 2);
    for (int i = 0; i < 3; i++)
        ck_assert_float_eq(v->items[i], 2.0f);
    free_vector(v);
}
END_TEST

START_TEST(test_standard_dot_product)
{
    Vector *u = create_dummy_real_vector(1.0f);
    Vector *v = create_dummy_real_vector(1.0f);
    int dot_prod = dot_product(u, v);
    ck_assert_float_eq(dot_prod, 3.0f);
    free_vector(u); free_vector(v);
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
}
END_TEST

START_TEST(test_correct_complex_absolute_value)
{
    float _Complex z = 2.0f + 1.0f * I;
    float cabs = complex_abs(z);
    ck_assert_float_eq(cabs, sqrt(pow(2,2)+pow(1,2)));
}
END_TEST

START_TEST(test_standard_L1_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = L1_norm(u);
    ck_assert_float_eq(n, 6.0f);
    free_vector(u);
}
END_TEST

START_TEST(test_standard_L2_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = L2_norm(u);
    ck_assert_float_eq(n, sqrt(12));
    free_vector(u);
}
END_TEST

START_TEST(test_first_level_Lp_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = Lp_norm(u, 1);
    ck_assert_float_eq(n, 6.0f);
    free_vector(u);
}
END_TEST

START_TEST(test_second_level_Lp_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = Lp_norm(u, 2);
    ck_assert_float_eq(n, sqrt(12));
    free_vector(u);
}
END_TEST

START_TEST(test_third_level_Lp_norm)
{
    Vector *u = create_dummy_real_vector(2.0f);
    float n = Lp_norm(u, 3);
    ck_assert_float_eq(n, pow(24, 1/3));
    free_vector(u);
}
END_TEST


Suite *vector_suite(void)
{
    Suite *s = suite_create("Core");
    
    TCase *tc_vector_operations = tcase_create("Vector Functions");
    tcase_add_test(tc_vector_operations, test_empty_vector_is_created_correctly);
    tcase_add_test(tc_vector_operations, test_new_vector_is_updated_correctly);
    tcase_add_test(tc_vector_operations, test_standard_vector_addition);
    tcase_add_test(tc_vector_operations, test_standard_vector_subtraction);
    tcase_add_test(tc_vector_operations, test_standard_scalar_multiplication);
    tcase_add_test(tc_vector_operations, test_standard_dot_product);
    tcase_add_test(tc_vector_operations, test_standard_vector_product);
    tcase_add_test(tc_vector_operations, test_standard_L1_norm);
    tcase_add_test(tc_vector_operations, test_standard_L2_norm);
    tcase_add_test(tc_vector_operations, test_first_level_Lp_norm);
    tcase_add_test(tc_vector_operations, test_second_level_Lp_norm);
    tcase_add_test(tc_vector_operations, test_third_level_Lp_norm);
    suite_add_tcase(s, tc_vector_operations);

    TCase *tc_vector_helpers = tcase_create("Vector Helpers");
    tcase_add_test(tc_vector_helpers, test_correct_complex_absolute_value);
    suite_add_tcase(s, tc_vector_helpers);
    return s;
}

int main(void) {
    int nb_fails;
    Suite *s = vector_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    nb_fails = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nb_fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}