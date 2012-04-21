
#include "ant.h"

#include "ant_colony.h"
#include "food.h"

Ant::Ant(AntColony *team, float x, float y) {
    ANT_SPEED = rand_range(1.0f, 8.0f);
    
    this->team = team;
    this->SetPosition(x, y);
    this->SetColor(team->color);
    this->level = LEVEL_0;
    this->points = 0;
    this->state = STATE_SEEK_FOOD;
    this->target = NULL;
}


void Ant::update(float dt) {
    switch (this->state) {
        case STATE_SEEK_FOOD:
            seekFood();
            break;
            
        case STATE_RETURN_FOOD:
            returnFood();
            break;
            
        case STATE_ATTACK:
            attackTarget();
            break;
            
        case STATE_HELP_ANT:
            helpAnt();
            break;
            
        case STATE_FOLLOW_TRAIL:
            followTrail();
            break;
            
        default:
            this->state = STATE_SEEK_FOOD;
            break;
    }
    
    
    
    // Update position from velocity
    this->SetPosition(this->GetPosition() + (*(this->velocity)) * dt);
}
 
/**
 * The ant does a random walk whilst looking for food
 */
void Ant::seekFood() {
    if(target) {
    //    Vector2 *heading = new Vector2(target->GetPosition());
    //    *heading -= this->GetPosition();
    //    this->velocity = heading;        
    } else {
        
        Vector2 *heading = new Vector2(this->GetPosition());
        *heading -= this->team->GetPosition();
        heading->Normalize();
        
        *heading += Vector2::Vector2(
                                     ANT_SPEED * ((1.0f * rand() / RAND_MAX) - 0.5f),
                                     ANT_SPEED * ((1.0f * rand() / RAND_MAX) - 0.5f)
                                     );
        
        this->velocity = heading;
        
        
        // Check if we can see any food
        for(std::vector<Food *>::iterator i=theGame.foodbits.begin(); i!=theGame.foodbits.end(); i++) {
            if(fabs((*i)->GetPosition().X - this->GetPosition().X) < SIGHT_RANGE) {
                if(fabs((*i)->GetPosition().Y - this->GetPosition().Y) < SIGHT_RANGE) {
                    Vector2 *tmp = new Vector2((*i)->GetPosition());
                    *tmp -= this->GetPosition();
                    
                    if((*tmp).Length() < SIGHT_RANGE) {
                        target = *i;
                    }
                }
            }
        }
    }
}

/**
 * Returns food to the nest, whilst leaving a trail
 */
void Ant::returnFood() {
    
}

/**
 * Moves towards and attacks the current target
 */
void Ant::attackTarget() {
    
}

/**
 * Helps the target ant with whatever task it is currently doing
 */
void Ant::helpAnt() {
    
}

/**
 * Follows the trail the ant has found
 */
void Ant::followTrail() {
    
}

void Ant::addXP(int points) {
    this->points += points;
    
    if(points > 100) {
        this->levelUp();
    }
}

void Ant::levelUp() {
    if(this->level == LEVEL_0) {
        this->level = LEVEL_1;
    } else if(this->level == LEVEL_1) {
        this->level = LEVEL_2;
    } else if(this->level == LEVEL_2) {
        this->level = LEVEL_3;
    } else if(this->level == LEVEL_3) {
        this->level = LEVEL_4;
    } else if(this->level == LEVEL_4) {
        this->level = LEVEL_5;
    } else if(this->level == LEVEL_5) {
        this->level = LEVEL_6;
    } else if(this->level == LEVEL_6) {
        this->level = LEVEL_7;
    } else {
        return;
    }
    
    this->points = points % 100;
}


void Ant::ReceiveMessage(Message *msg) {
    // TODO...
}


