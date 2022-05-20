#include <check.h>
#include "../C/vector.h"

START_TEST(test_empty_vector_creation)
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

START_TEST(test_update_vector)
{
    Vector *v = malloc(sizeof(Vector));
    init_vector(v, "V", 3);
    for (int i = 0; i < 3; i++)
        update_vector(v, 1.0f + 1.0f*I, i);
    for (int i = 0; i < 3; i++) {
        ck_assert_float_eq(creal(v->items[i]), 1.0f);
        ck_assert_float_eq(cimag(v->items[i]), 1.0f);
    }
    free_vector(v);
}
END_TEST

Suite *vector_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Core");

    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_empty_vector_creation);
    tcase_add_test(tc_core, test_update_vector);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int nb_fails;
    Suite *s;
    SRunner *sr;

    s = vector_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    nb_fails = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (nb_fails == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}