
#ifndef ANTGAME_MATH_UTILS_H
#define ANTGAME_MATH_UTILS_H

#include "stdafx.h"

float gauss_rand(float mean, float range);
float capped_gauss_rand(float min, float max, unsigned rolls=10, double stretch=2);
float rand_range(float lower, float upper);
float rand_float();
bool dist_check(Vector2 a, Vector2 b, float dist);

#endif


