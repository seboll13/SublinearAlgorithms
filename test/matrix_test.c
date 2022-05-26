#include <check.h>
#include "../C/matrix.h"

/**
 * @brief Create a dummy real matrix object
 * 
 * @param n value to fill in matrix for
 * @return Matrix* 3x3 fixed element matrix
 */
Matrix *create_dummy_real_matrix(float _Complex n) {
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 3);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            update_matrix(m, n, i, j);
    return m;
}

START_TEST(test_empty_matrix_is_created_correctly)
{
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 3);
    ck_assert_int_eq(m->rows, 3);
    ck_assert_int_eq(m->cols, 3);
    ck_assert_str_eq(m->name, "M");
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            ck_assert_float_eq(creal(m->items[j].items[i]), 0.0f);
            ck_assert_float_eq(cimag(m->items[j].items[i]), 0.0f);
        }
    }
    free_matrix(m);
    free(m);
}
END_TEST

START_TEST(test_new_matrix_is_updated_correctly)
{
    Matrix *m = create_dummy_real_matrix(1.0f);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            ck_assert_float_eq(m->items[j].items[i], 1.0f);
    free_matrix(m);
    free(m);
}
END_TEST

START_TEST(test_standard_matrix_addition)
{
    Matrix *m1 = create_dummy_real_matrix(1.0f);
    Matrix *m2 = create_dummy_real_matrix(1.0f);
    
    Matrix *m = matrix_add(m1, m2, true);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            ck_assert_float_eq(m->items[j].items[i], 2.0f);
    free_matrix(m1); free_matrix(m2); free_matrix(m);
    free(m1); free(m2); free(m);
}
END_TEST

START_TEST(test_standard_matrix_subtraction)
{
    Matrix *m1 = create_dummy_real_matrix(1.0f);
    Matrix *m2 = create_dummy_real_matrix(1.0f);
    
    Matrix *m = matrix_add(m1, m2, false);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            ck_assert_float_eq(m->items[j].items[i], 0.0f);
    free_matrix(m1); free_matrix(m2); free_matrix(m);
    free(m1); free(m2); free(m);
}
END_TEST

START_TEST(test_standard_matrix_multiplication)
{
    Matrix *m1 = create_dummy_real_matrix(1.0f);
    Matrix *m2 = create_dummy_real_matrix(2.0f);
    
    Matrix *m = matrix_mult(m1, m2);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            ck_assert_float_eq(m->items[j].items[i], 6.0f);
    free_matrix(m1); free_matrix(m2); free_matrix(m);
    free(m1); free(m2); free(m);
}
END_TEST

START_TEST(test_standard_hadamard_product)
{
    Matrix *m1 = create_dummy_real_matrix(1.0f);
    Matrix *m2 = create_dummy_real_matrix(2.0f);
    
    Matrix *m = hadamard_prod(m1, m2);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            ck_assert_float_eq(m->items[j].items[i], 2.0f);
    free_matrix(m1); free_matrix(m2); free_matrix(m);
    free(m1); free(m2); free(m);
}
END_TEST

START_TEST(test_standard_transpose)
{
    float _Complex vals[2] = {0.0f, 1.0f};
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 2);
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 3; i++)
            update_matrix(m, vals[j], i, j);
    
    Matrix *t = matrix_transpose(m);
    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 3; i++)
            ck_assert_float_eq(t->items[i].items[j], vals[j]);
    free_matrix(t); free_matrix(m);
    free(t); free(m);
}
END_TEST

START_TEST(test_standard_matrix_trace)
{
    Matrix *m = create_dummy_real_matrix(1.0f);
    float _Complex trace = matrix_trace(m);
    ck_assert_float_eq(trace, 3.0f);
    free_matrix(m);
    free(m);
}
END_TEST

START_TEST(test_standard_matrix_L1_norm)
{
    float _Complex vals[3] = {1.0f, 2.0f, 3.0f};
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 3);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            update_matrix(m, vals[i], i, j);
    ck_assert_float_eq(matrix_L1_norm(m), 9.0f);
    free_matrix(m);
    free(m);
}
END_TEST

START_TEST(test_standard_matrix_Linf_norm)
{
    float _Complex vals[3] = {1.0f, 2.0f, 3.0f};
    Matrix *m = malloc(sizeof(Matrix));
    init_matrix(m, "M", 3, 3);
    for (int j = 0; j < 3; j++)
        for (int i = 0; i < 3; i++)
            update_matrix(m, vals[i], i, j);
    ck_assert_float_eq(matrix_Linf_norm(m), 6.0f);
    free_matrix(m);
    free(m);
}
END_TEST