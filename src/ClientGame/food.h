

#ifndef ANTGAME_FOOD_H
#define ANTGAME_FOOD_H

#include "ant_target.h"

// Forward-declare Ant
class Ant;

class Food : public AntTarget {
public:
    Food(float x, float y);
    
    virtual void Update(float dt);
    
    Ant *carriedBy;

private:
    std::string FOOD_COLOR;
};


#endif

