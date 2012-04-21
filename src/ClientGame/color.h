
#ifndef ANTGAME_COLOR_H
#define ANTGAME_COLOR_H


#include <cstdlib>
#include <cmath>

/**
 * A small wrapper around the default color class that adds some simple
 * methods for mutating colors
 */
class MyColor : public Color {
public:
    MyColor(Color c);
    Color mutate(float ammount);
    
    Color setR(float r);
    Color setG(float g);
    Color setB(float b);
    Color setA(float a);
    
private:
    float smallRand(float ammount);

};


#endif

