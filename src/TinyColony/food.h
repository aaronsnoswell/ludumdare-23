

#ifndef ANTGAME_FOOD_H
#define ANTGAME_FOOD_H

#include "game.h"
#include "ant_target.h"

#define VALUE_STANDARD  5;
#define VALUE_EXTRA     10;

class Food : public AntTarget {
public:
    Food(float x, float y);
    
    bool consumed;
    virtual void Remove();
    
    int value;
    bool being_carried;

private:
    std::string FOOD_COLOR;
};


#endif

