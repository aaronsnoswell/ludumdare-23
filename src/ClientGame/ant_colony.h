
#ifndef ANTGAME_ANT_COLONY_H
#define ANTGAME_ANT_COLONY_H

#include "ant.h"
#include "ant_target.h"

#define FOOD_START          150
#define ANT_SPAWN_TIME      5
#define ANT_COST            10

#define NEST_DROP_RANGE     2

// Forward-declare classes
class Food;

/**
 * A class representing a colony of ants
 */
class AntColony : public AntTarget {
public:
    AntColony(std::string name, std::string color, float x, float y);
    
    virtual void addFood(Food *f);
    virtual void Update(float dt);
    virtual void SpawnAnt();
    virtual void ForceSpawnAnt();
    
    std::vector<Ant *> ants;
    
    friend class Ant;
    
    int food_store;
    
protected:
    std::string name;
    Color color;
    bool _spawning;
    
    virtual void ReceiveMessage(Message *m);
};

#endif

