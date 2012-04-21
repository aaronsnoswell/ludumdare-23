

#ifndef ANTGAME_ANT_H
#define ANTGAME_ANT_H

#include "game.h"
#include "math_utils.h"
#include "ant_target.h"

#include <iostream>
#include <string>

// The ant levels
#define LEVEL_0     "Youngling"
#define LEVEL_1     "Tenderfoot"
#define LEVEL_2     "Anklebiter"
#define LEVEL_3     "Sure-jaw"
#define LEVEL_4     "Worker"
#define LEVEL_5     "Toiler"
#define LEVEL_6     "Soldier"
#define LEVEL_7     "Commander"

// The ant states
#define STATE_SEEK_FOOD     0
#define STATE_RETURN_FOOD   1
#define STATE_ATTACK        2
#define STATE_HELP_ANT      3
#define STATE_FOLLOW_TRAIL  4

// How far ants can see
#define SIGHT_RANGE         10

// Forward-define AntColony
class AntColony;

/**
 * An ant, AI controlled
 */
class Ant : public AntTarget, public MessageListener {
public:
    Ant(AntColony *team, float x, float y);
    
    void update(float dt);
    void addXP(int points);
    void levelUp();
    
	virtual void ReceiveMessage(Message *message);
    
private:
    AntColony *team;
    float health;
    float stamina;
    std::string level;
    int points;
    int state;
    AntTarget *target;
    
    float ANT_SPEED;
    
    Vector2 *velocity;
    
    // The different movement states
    void seekFood();
    void returnFood();
    void attackTarget();
    void helpAnt();
    void followTrail();
};


#endif

