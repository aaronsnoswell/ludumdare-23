


#ifndef ANTGAME_GAME_H
#define ANTGAME_GAME_H

#include "color.h"
#include "math_utils.h"

// These dimensions are actually for half the board size
#define WORLD_WIDTH         100
#define WORLD_HEIGHT        75

#define GROUND_COLOR        "#4e3317"

#define FOOD_PEICES         100
#define FOOD_CACHES         5

// The half-length of a day, in s
#define DAY_CYCLE_LENGTH    240


#define LAYER_GROUND        -2
#define LAYER_GRID          -1
#define LAYER_NEST          0
#define LAYER_FOOD          1
#define LAYER_ANT           2

#define LAYER_TIMESHADE     50
#define LAYER_SKY           51

// Forward-declare classes
class AntColony;
class Food;
class Player;

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
    
    AntColony *blue_colony, *red_colony;
    
    Player *player;
    
    void ForceAddFoodBit(float x, float y);
    std::vector<Food *> foodbits;
    
protected:
    Game();
	static Game *_Game;
    
    Actor *ground;
    Actor *time_shade;
    
    std::vector<std::pair<Vector2 *, int> > _food_locs;
    
};


#endif
