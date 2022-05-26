#include <check.h>
#include "vector_test.c"
#include "matrix_test.c"
#include "tensor_test.c"

Suite *vector_suite(void) {
    Suite *s = suite_create("Vector");
    
    TCase *tc_vector_operations = tcase_create("Vector functions");
    tcase_add_test(tc_vector_operations, test_empty_vector_is_created_correctly);
    tcase_add_test(tc_vector_operations, test_new_vector_is_updated_correctly);
    tcase_add_test(tc_vector_operations, test_standard_vector_addition);
    tcase_add_test(tc_vector_operations, test_standard_vector_subtraction);
    tcase_add_test(tc_vector_operations, test_standard_scalar_multiplication);
    tcase_add_test(tc_vector_operations, test_standard_dot_product);
    tcase_add_test(tc_vector_operations, test_standard_vector_product);
    tcase_add_test(tc_vector_operations, test_standard_scalar_projection);
    tcase_add_test(tc_vector_operations, test_standard_vector_projection);
    tcase_add_test(tc_vector_operations, test_standard_vector_L1_norm);
    tcase_add_test(tc_vector_operations, test_standard_vector_L2_norm);
    tcase_add_test(tc_vector_operations, test_first_level_vector_Lp_norm);
    tcase_add_test(tc_vector_operations, test_second_level_vector_Lp_norm);
    tcase_add_test(tc_vector_operations, test_third_level_vector_Lp_norm);
    suite_add_tcase(s, tc_vector_operations);

    TCase *tc_vector_helpers = tcase_create("Vector helpers");
    tcase_add_test(tc_vector_helpers, test_correct_complex_absolute_value);
    tcase_add_test(tc_vector_helpers, test_orthogonality_for_two_orthogonal_vectors);
    tcase_add_test(tc_vector_helpers, test_orthogonality_for_two_non_orthogonal_vectors);
    tcase_add_test(tc_vector_helpers, test_standard_radians_to_degrees);
    tcase_add_test(tc_vector_helpers, test_correct_angle_between_vector);
    tcase_add_test(tc_vector_helpers, test_orthogonality_yields_right_angle);
    suite_add_tcase(s, tc_vector_helpers);
    return s;
}

Suite *matrix_suite(void) {
    Suite *s = suite_create("Matrix");
    
    TCase *tc_matrix_operations = tcase_create("Matrix functions");
    tcase_add_test(tc_matrix_operations, test_empty_matrix_is_created_correctly);
    tcase_add_test(tc_matrix_operations, test_new_matrix_is_updated_correctly);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_addition);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_subtraction);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_multiplication);
    tcase_add_test(tc_matrix_operations, test_standard_hadamard_product);
    tcase_add_test(tc_matrix_operations, test_standard_transpose);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_trace);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_L1_norm);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_Linf_norm);
    suite_add_tcase(s, tc_matrix_operations);
    return s;
}

Suite *tensor_suite(void) {
    Suite *s = suite_create("Tensor");

    TCase *tc_tensor_operations = tcase_create("Tensor functions");
    tcase_add_test(tc_tensor_operations, test_empty_tensor_is_created_correctly);
    tcase_add_test(tc_tensor_operations, test_new_tensor_is_updated_correctly);
    tcase_add_test(tc_tensor_operations, test_standard_tensor_addition);
    tcase_add_test(tc_tensor_operations, test_standard_tensor_subtraction);
    tcase_add_test(tc_tensor_operations, test_standard_tensor_scalar_multiplication);
    suite_add_tcase(s, tc_tensor_operations);
    return s;
}

int main(void) {
    int nb_fails;
    Suite *s_vector = vector_suite();
    Suite *s_matrix = matrix_suite();
    Suite *s_tensor = tensor_suite();
    SRunner *sr_vector = srunner_create(s_vector);
    SRunner *sr_matrix = srunner_create(s_matrix);
    SRunner *sr_tensor = srunner_create(s_tensor);

    srunner_run_all(sr_vector, CK_NORMAL);
    srunner_run_all(sr_matrix, CK_NORMAL);
    srunner_run_all(sr_tensor, CK_NORMAL);
    nb_fails = srunner_ntests_failed(sr_vector) + srunner_ntests_failed(sr_matrix) + srunner_ntests_failed(sr_tensor);
    srunner_free(sr_vector);
    srunner_free(sr_matrix);
    srunner_free(sr_tensor);
    return (nb_fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}