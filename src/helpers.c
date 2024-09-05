#include "helpers.h"

/**
 * @brief helper to convert radians to degrees
 * 
 * @param radians 
 * @return float 
 */
float radians_to_degrees(float radians) {
    return (float) 180 * fmod(radians, 2*M_PI) / M_PI;
}

/**
 * @brief return the complex absolute value of some complex number
 * 
 * @param z complex number
 * @return float L2 distance on the complex plane (i.e. norm of a complex)
 */
float complex_abs(float _Complex z) {
    return sqrt(pow(creal(z),2) + pow(cimag(z),2));
}