
#ifndef ANTGAME_PLAYER_H
#define ANTGAME_PLAYER_H

#include "ant.h"


class Player : public Ant {
public:
    Player(AntColony *team, float x, float y);
    
    virtual void Update(float dt);
    virtual void ReceiveMessage(Message *m);
    virtual void bite(Ant *a);
    
private:
    bool SPACE_PRESSED, LEFT_PRESSED, RIGHT_PRESSED, UP_PRESSED, DOWN_PRESSED;
    
    void die();
    
};




#endif

