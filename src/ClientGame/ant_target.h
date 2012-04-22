

#ifndef ANTGAME_ANT_TARGET_H
#define ANTGAME_ANT_TARGET_H

class Ant;

/**
 * A simple class for anything that can be targetted by an ant
 */
class AntTarget : public Actor {
public:
    AntTarget();
    
    virtual void bitten();
};



#endif

