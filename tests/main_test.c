#include <check.h>
#include "vector_test.c"
#include "matrix_test.c"
#include "tensor_test.c"
#include "helpers_test.c"

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
    tcase_add_test(tc_vector_helpers, test_orthogonality_for_two_orthogonal_vectors);
    tcase_add_test(tc_vector_helpers, test_orthogonality_for_two_non_orthogonal_vectors);
    tcase_add_test(tc_vector_helpers, test_orthogonality_yields_right_angle);
    tcase_add_test(tc_vector_helpers, test_correct_angle_between_vector);
    tcase_add_test(tc_vector_helpers, test_collinearity_between_two_parallel_vectors);
    tcase_add_test(tc_vector_helpers, test_collinearity_between_two_non_parallel_vectors);
    tcase_add_test(tc_vector_helpers, test_perpendicularity_between_two_orthogonal_vectors);
    tcase_add_test(tc_vector_helpers, test_perpendicularity_between_two_non_orthogonal_vectors);
    tcase_add_test(tc_vector_helpers, test_equality_between_two_identical_vectors);
    tcase_add_test(tc_vector_helpers, test_equality_between_two_different_vectors);
    tcase_add_test(tc_vector_helpers, test_oppositeness_between_two_opposite_vectors);
    tcase_add_test(tc_vector_helpers, test_oppositeness_between_two_non_opposite_vectors);
    tcase_add_test(tc_vector_helpers, test_vector_of_integers);
    tcase_add_test(tc_vector_helpers, test_vector_of_floats);
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
    tcase_add_test(tc_matrix_operations, test_standard_matrix_scalar_mult);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_multiplication);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_power);
    tcase_add_test(tc_matrix_operations, test_standard_hadamard_product);
    tcase_add_test(tc_matrix_operations, test_standard_kroenecker_product);
    tcase_add_test(tc_matrix_operations, test_standard_transpose);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_cofactor);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_adjoint);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_inverse);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_determinant);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_trace);
    //tcase_add_test(tc_matrix_operations, test_standard_matrix_rank);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_L1_norm);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_Linf_norm);
    tcase_add_test(tc_matrix_operations, test_standard_matrix_frobenius_norm);
    suite_add_tcase(s, tc_matrix_operations);

    TCase *tc_matrix_helpers = tcase_create("Matrix helpers");
    tcase_add_test(tc_matrix_helpers, test_symmetry_of_symmetric_matrix);
    tcase_add_test(tc_matrix_helpers, test_symmetry_of_asymmetric_matrix);
    tcase_add_test(tc_matrix_helpers, test_diagonality_of_diagonal_matrix);
    tcase_add_test(tc_matrix_helpers, test_diagonality_of_non_diagonal_matrix);
    tcase_add_test(tc_matrix_helpers, test_matrix_with_line_of_all_zeroes);
    tcase_add_test(tc_matrix_helpers, test_matrix_without_line_of_all_zeroes);
    tcase_add_test(tc_matrix_helpers, test_matrix_of_integers);
    tcase_add_test(tc_matrix_helpers, test_matrix_of_floats);
    tcase_add_test(tc_matrix_helpers, test_stochasticity_of_stochastic_matrix);
    tcase_add_test(tc_matrix_helpers, test_stochasticity_of_non_stochastic_matrix);
    tcase_add_test(tc_matrix_helpers, test_doubly_stochasticity_of_doubly_stochastic_matrix);
    tcase_add_test(tc_matrix_helpers, test_doubly_stochasticity_of_non_doubly_stochastic_matrix);
    suite_add_tcase(s, tc_matrix_helpers);
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
    tcase_add_test(tc_tensor_operations, test_standard_tensor_elementwise_multiplication);
    suite_add_tcase(s, tc_tensor_operations);
    return s;
}

Suite *helpers_suite(void) {
    Suite *s = suite_create("Helpers");

    TCase *tc_helper_functions = tcase_create("Helper functions");
    tcase_add_test(tc_helper_functions, test_correct_complex_absolute_value);
    tcase_add_test(tc_helper_functions, test_standard_radians_to_degrees);
    suite_add_tcase(s, tc_helper_functions);
    return s;
}

int main(void) {
    int nb_fails;
    Suite *s_vector = vector_suite();
    Suite *s_matrix = matrix_suite();
    Suite *s_tensor = tensor_suite();
    Suite *s_helpers = helpers_suite();
    SRunner *sr_vector = srunner_create(s_vector);
    SRunner *sr_matrix = srunner_create(s_matrix);
    SRunner *sr_tensor = srunner_create(s_tensor);
    SRunner *sr_helpers = srunner_create(s_helpers);

    srunner_run_all(sr_vector, CK_NORMAL);
    srunner_run_all(sr_matrix, CK_NORMAL);
    srunner_run_all(sr_tensor, CK_NORMAL);
    srunner_run_all(sr_helpers, CK_NORMAL);
    nb_fails = srunner_ntests_failed(sr_vector) \
        + srunner_ntests_failed(sr_matrix) \
        + srunner_ntests_failed(sr_tensor) \
        + srunner_ntests_failed(sr_helpers);
    srunner_free(sr_vector);
    srunner_free(sr_matrix);
    srunner_free(sr_tensor);
    srunner_free(sr_helpers);
    return (nb_fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}