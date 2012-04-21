
#ifndef ANTGAME_ANT_COLONY_H
#define ANTGAME_ANT_COLONY_H

#include "ant.h"
#include "ant_target.h"

/**
 * A class representing a colony of ants
 */
class AntColony : public AntTarget {
public:
    AntColony(std::string name, std::string color, float x, float y);
    
    friend class Ant;
    
protected:
    std::string name;
    Color color;
};

#endif

