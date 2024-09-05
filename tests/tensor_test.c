#include <check.h>
#include "../src/tensor.h"


/**
 * @brief Create a dummy real tensor object
 * 
 * @param n value to fill in tensor for
 * @return Tensor* 3x3x2 fixed element tensor
 */
Tensor *create_dummy_real_tensor(float _Complex n) {
    Tensor *T = malloc(sizeof(Tensor));
    init_tensor(T, "T", 3, 3, 2);
    for (int n_3 = 0; n_3 < 2; n_3++)
        for (int n_2 = 0; n_2 < 3; n_2++)
            for (int n_1 = 0; n_1 < 3; n_1++)
                update_tensor(T, n, n_1, n_2, n_3);
    return T;
}

START_TEST(test_empty_tensor_is_created_correctly)
{
    Tensor *T = malloc(sizeof(Tensor));
    init_tensor(T, "T", 3, 3, 2);
    ck_assert_int_eq(T->rows, 3);
    ck_assert_int_eq(T->cols, 3);
    ck_assert_int_eq(T->depth, 2);
    for (int n_3 = 0; n_3 < 2; n_3++) {
        for (int n_2 = 0; n_2 < 3; n_2++) {
            for (int n_1 = 0; n_1 < 3; n_1++) {
                ck_assert_float_eq(creal(T->items[n_3].items[n_2].items[n_1]), 0.0f);
                ck_assert_float_eq(cimag(T->items[n_3].items[n_2].items[n_1]), 0.0f);
            }
        }
    }
    free_tensor(T);
    free(T);
}
END_TEST

START_TEST(test_new_tensor_is_updated_correctly)
{
    Tensor *T = create_dummy_real_tensor(1.0f);
    for (int n_3 = 0; n_3 < 2; n_3++)
        for (int n_2 = 0; n_2 < 3; n_2++)
            for (int n_1 = 0; n_1 < 3; n_1++)
                ck_assert_float_eq(T->items[n_3].items[n_2].items[n_1], 1.0f);
    free_tensor(T);
    free(T);
}
END_TEST

START_TEST(test_standard_tensor_addition)
{
    Tensor *E = create_dummy_real_tensor(1.0f);
    Tensor *F = create_dummy_real_tensor(1.0f);
    
    Tensor *T = tensor_add(E, F, true);
    for (int n_3 = 0; n_3 < 2; n_3++)
        for (int n_2 = 0; n_2 < 3; n_2++)
            for (int n_1 = 0; n_1 < 3; n_1++)
                ck_assert_float_eq(T->items[n_3].items[n_2].items[n_1], 2.0f);
    free_tensor(E); free_tensor(F); free_tensor(T);
    free(E); free(F); free(T);
}
END_TEST

START_TEST(test_standard_tensor_subtraction)
{
    Tensor *E = create_dummy_real_tensor(1.0f);
    Tensor *F = create_dummy_real_tensor(1.0f);
    
    Tensor *T = tensor_add(E, F, false);
    for (int n_3 = 0; n_3 < 2; n_3++)
        for (int n_2 = 0; n_2 < 3; n_2++)
            for (int n_1 = 0; n_1 < 3; n_1++)
                ck_assert_float_eq(T->items[n_3].items[n_2].items[n_1], 0.0f);
    free_tensor(E); free_tensor(F); free_tensor(T);
    free(E); free(F); free(T);
}
END_TEST

START_TEST(test_standard_tensor_scalar_multiplication)
{
    Tensor *T = create_dummy_real_tensor(1.0f);
    
    Tensor *V = tensor_scalar_mult(T, 2.0f);
    for (int n_3 = 0; n_3 < 2; n_3++)
        for (int n_2 = 0; n_2 < 3; n_2++)
            for (int n_1 = 0; n_1 < 3; n_1++)
                ck_assert_float_eq(V->items[n_3].items[n_2].items[n_1], 2.0f);
    free_tensor(V); free_tensor(T);
    free(V); free(T);
}
END_TEST

START_TEST(test_standard_tensor_elementwise_multiplication)
{
    Tensor *E = create_dummy_real_tensor(1.0f);
    Tensor *F = create_dummy_real_tensor(1.0f);
    
    Tensor *T = tensor_mult(E, F);
    for (int n_3 = 0; n_3 < 2; n_3++)
        for (int n_2 = 0; n_2 < 3; n_2++)
            for (int n_1 = 0; n_1 < 3; n_1++)
                ck_assert_float_eq(T->items[n_3].items[n_2].items[n_1], 1.0f);
    free_tensor(E); free_tensor(F); free_tensor(T);
    free(E); free(F); free(T);
}