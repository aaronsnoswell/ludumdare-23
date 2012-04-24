
#include "stdafx.h"

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
#define STATE_RUN_HOME      5

// How far ants can see
#define SIGHT_RANGE         10
#define BITE_RANGE          1.5f
#define BITE_HURT           5

#define ANT_HEAD_LENGTH     0.75f
#define ANT_START_HEALTH    15
#define BASE_ANT_SIZE       1.8f
#define BITE_DELAY          1.0f

// Forward-define AntColony
class AntColony;

/**
 * An ant, AI controlled
 */
class Ant : public AntTarget {
public:
    Ant(AntColony *team, float x, float y);
    
    virtual void Update(float dt);
    virtual void addXP(int points);
    virtual void levelUp();
    
	virtual void ReceiveMessage(Message *message);
    virtual void bitten();
    
    friend class Player;
    
    std::string level;
    float health_max;
    float health;
    int experience;
    
    TextActor *my_state;
    
    bool dead;
    void die();
    
protected:
    AntColony *team;
    int state;
    AntTarget *target;
    Food *carrying;
    bool _can_bite;
    
    float ANT_SPEED;
    float ANT_HEALTH;
    
    Vector2 velocity;
    static bool DEBUG_DISPLAY;
    
    // The different movement states
    bool _seeking_food;
    void seekFood();
    void returnFood();
    void attackTarget();
    void helpAnt();
    void followTrail();
    void runHome();
    
    void dropFood();
    
    Vector2 GetHeadPosition();
    
};


#endif


