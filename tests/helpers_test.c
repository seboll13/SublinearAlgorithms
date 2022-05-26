#include <check.h>
#include "../C/helpers.h"

START_TEST(test_correct_complex_absolute_value) 
{
    float _Complex z = 2.0f + 1.0f * I;
    float cabs = complex_abs(z);
    ck_assert_float_eq(cabs, sqrt(pow(2,2)+pow(1,2)));
}
END_TEST

START_TEST(test_standard_radians_to_degrees)
{
    ck_assert_float_eq(180.0f, radians_to_degrees(M_PI));
    ck_assert_float_eq(180.0f, radians_to_degrees(3*M_PI));
}
END_TEST