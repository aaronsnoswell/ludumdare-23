


#ifndef ANTGAME_GAME_H
#define ANTGAME_GAME_H

#include "color.h"
#include "math_utils.h"

#define WORLD_WIDTH         100
#define WORLD_HEIGHT        75

#define GROUND_COLOR        "#4e3317"
#define GROUND_TILES        100

#define COLONY_START_SIZE   50

#define FOOD_PEICES         100

// The half-length of a day, in s
#define DAY_CYCLE_LENGTH    240

// Forward-declare Ant and Food
class Ant;
class Food;

/**
 * The Game class handles all things gamely
 */

#define theGame Game::GetInstance()

class Game : public MessageListener {
public:
    static Game &GetInstance();
    
    virtual void Start(std::vector<Renderable *> objects);
	virtual void Update(float dt);
	virtual void ReceiveMessage(Message *message);
    
    
    std::vector<Ant *> ants;
    
    std::vector<Food *> foodbits;
    
protected:
    Game();
	static Game *_Game;
    
    std::vector<Actor *> ground_tiles;
    FullScreenActor *ground;
    FullScreenActor *time_shade;
    
};


#endif
