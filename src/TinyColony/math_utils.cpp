
#include "stdafx.h"

#include "math_utils.h"

#include <cstdlib>
#include <cmath>

/**
 * Returns an approximate gauusian distributed random number near the
 * target value
 */
float gauss_rand(float mean, float range) {
    return capped_gauss_rand(0, range * 2) + mean - range;
}


float capped_gauss_rand(float min, float max, unsigned rolls, double stretch) {
    if(stretch < 1) stretch = 1;
    if(rolls <= 1) {
        return rand() % int(max - min + 1) + min;
    }
    
    float result;
    float span = max - min + 1;
    float stretchedSpan = span * stretch + .5;
    do {
        float total = 0;
        for(unsigned i = 0; i != rolls; ++i) {
            total += rand() % int(stretchedSpan);
        }
        
        result = double(total) / rolls - (stretch - 1) * span / 2 + 0.5;
    } while(result > max);
    
    return result + min;
}


/**
 * Returns a randomly distributed float between the upper and lower bounds
 */
float rand_range(float lower, float upper) {
    float range = upper - lower;
    return range * rand() / RAND_MAX + lower;
}


/**
 * Returns a randomly distributed float between 0 and 1
 */
float rand_float() {
    return rand_range(0.0f, 1.0f);
}

/**
 * Checks if the distance between points a and b is < dist in an efficient
 * manner
 */
bool dist_check(Vector2 a, Vector2 b, float dist) {
    if(fabs(a.X - b.X) < dist) {
        if(fabs(a.Y - b.Y) < dist) {
            if(Vector2::Vector2(a - b).Length() < dist) {
                return true;
            }
        }
    }
    return false;
}






