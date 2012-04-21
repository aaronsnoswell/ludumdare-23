
#include "math_utils.h"


/**
 * Returns an approximate gauusian distributed random number near the
 * target value (variance = 0.5)
 */
float gauss_rand(float target) {
    // Approximate a normal distribution
    float sum = 0.0f;
    for(int i=0; i<100; i++) sum += 1.0f * rand() / RAND_MAX;
    sum -= 50.0f;
    sum /= 50.0f;
    
    // Scale the normal distribution to be near ammount
    sum = sqrt(0.5f) * sum + target;
    
    return sum;
}


/**
 * Returns a randomly distributed float between the upper and lower bounds
 */
float rand_range(float lower, float upper) {
    float range = upper - lower;
    return range * rand() / RAND_MAX + lower;
}
