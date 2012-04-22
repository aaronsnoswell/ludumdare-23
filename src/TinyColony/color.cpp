


#include "color.h"

MyColor::MyColor(Color c) {
    this->R = c.R;
    this->G = c.G;
    this->B = c.B;
    this->A = c.A;
}

/**
 * Returns a random number, of magnitude ~ammount.
 * The returned value will be -1 <= ret <= 1
 */
float MyColor::smallRand(float ammount) {
    // Approximate a normal distribution
    float sum = 0.0f;
    for(int i=0; i<100; i++) sum += 1.0f * rand() / RAND_MAX;
    sum -= 50.0f;
    sum /= 50.0f;
    
    // Scale the normal distribution to be near ammount
    sum = (sqrt(0.5f) * sum + 1.0f) * ammount;
    
    // Cap the value
    sum = fmin(1.0f, fmax(sum, -1.0f));
    
    // Either return +ve or -ve
    if((1.0f * rand() / RAND_MAX) > 0.5f) sum *= -1;
    
    return sum;
}

/**
 * Mutates this color slightly based on ammount
 * Returns a self-reference for chaining
 */
Color MyColor::mutate(float ammount) {
    this->R = this->R + smallRand(ammount);
    this->G = this-> G + smallRand(ammount);
    this->B = this->B + smallRand(ammount);
    return *this;
}

Color MyColor::setR(float r) {
    this->R = r;
    return *this;
}

Color MyColor::setG(float g) {
    this->G = g;
    return *this;
}

Color MyColor::setB(float b) {
    this->B = b;
    return *this;
}

Color MyColor::setA(float a) {
    this->A = a;
    return *this;
}





