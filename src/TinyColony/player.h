
#include "stdafx.h"

#ifndef ANTGAME_PLAYER_H
#define ANTGAME_PLAYER_H

#include "ant.h"


class Player : public Ant {
public:
    Player(AntColony *team, float x, float y);
    
    virtual void Update(float dt);
    virtual void ReceiveMessage(Message *m);
    virtual void bitten();
    virtual void addXP(int points);
    virtual void levelUp();
    
    
private:
    bool SPACE_PRESSED, LEFT_PRESSED, RIGHT_PRESSED, UP_PRESSED, DOWN_PRESSED;
    
    void die();
    
    AngelSampleHandle bite, hurt, xp, levelup;
    
};




#endif

