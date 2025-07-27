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