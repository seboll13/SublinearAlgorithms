#include <check.h>
#include "../src/helpers.h"

START_TEST(test_standard_radians_to_degrees)
{
    ck_assert_float_eq(180.0f, radians_to_degrees(M_PI));
    ck_assert_float_eq(180.0f, radians_to_degrees(3*M_PI));
}
END_TEST