
#include "stdafx.h"

#include "ant.h"

#include <sstream>

#include "ant_colony.h"
#include "food.h"
#include "player.h"


bool Ant::DEBUG_DISPLAY = false;

Ant::Ant(AntColony *team, float x, float y) {
    ANT_SPEED = rand_range(2.0f, 3.0f);
    ANT_HEALTH = ANT_START_HEALTH;
    
    this->team = team;
    this->SetPosition(x, y);
    this->SetColor(team->color);
    this->SetSize(BASE_ANT_SIZE);
    this->SetSprite("Resources/Images/ant.png");
    
    this->level = LEVEL_0;
    this->health = ANT_HEALTH;
    this->health_max = ANT_HEALTH;
    this->experience = 0;
    this->state = STATE_SEEK_FOOD;
    this->target = NULL;
    this->carrying = NULL;
    this->dead = false;
    this->_can_bite = true;
    
    this->my_state = new TextActor("Console", "");
    this->my_state->SetColor(1, 1, 1);
    theWorld.Add(this->my_state);
    
    this->_seeking_food = false;
    
    theSwitchboard.SubscribeTo(this, "AntChangeDirection");
    theSwitchboard.SubscribeTo(this, "AntCanBite");
    theSwitchboard.SubscribeTo(this, "F1Pressed");
}


void Ant::Update(float dt) {
    Actor::Update(dt);
    
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
            
        case STATE_RUN_HOME:
            runHome();
            break;
            
        default:
            this->state = STATE_SEEK_FOOD;
            break;
    }
    
    
    /* Check for potential targets in this order:
     * Friendly ants > Insects > Player > Enemy Ants > Food
     */
    
    // Check for friendly ants (and help them attack if needed)
    /*
    if(this->target == NULL && this->carrying == NULL) {
        for(std::vector<Ant *>::iterator i=this->team->ants.begin(); i!=this->team->ants.end(); i++) {
            if(dist_check(this->GetPosition(), (*i)->GetPosition(), SIGHT_RANGE)) {
                // If our friend here is attacking something other than food, go help him
                if((*i)->target != NULL && ((Food *) *i) == NULL) {
                    this->target = (*i)->target;
                    this->state = STATE_ATTACK;
                }
            }
        }
    }
     */
    
    // Check for insects
    // TODO ajs 22/04/12
    
    // Check for enemy ants and attack them
    AntColony *enemy_team = (theGame.blue_colony == this->team) ? theGame.red_colony : theGame.blue_colony;
    if(this->target == NULL || this->target == this->team) {
        for(std::vector<Ant *>::iterator i=enemy_team->ants.begin(); i!=enemy_team->ants.end(); i++) {
            if(dist_check(this->GetPosition(), (*i)->GetPosition(), SIGHT_RANGE)) {
                // Go attack the ant
                this->target = (*i);
                this->dropFood();
                this->state = STATE_ATTACK;
            }
        }
    }
    
    // Check for food
    if(this->target == NULL && this->carrying == NULL) {
        for(std::vector<Food *>::iterator i=theGame.foodbits.begin(); i!=theGame.foodbits.end(); i++) {
            if(dist_check(this->GetPosition(), (*i)->GetPosition(), SIGHT_RANGE) && !((*i)->being_carried)) {
                // Target the food
                this->target = (*i);
                this->state = STATE_ATTACK;
            }
        }
    }
    
    // If we're about to die, run home!
    if(this->health <= 5) {
        if(this->carrying != NULL) {
            this->carrying->being_carried = false;
            this->carrying = NULL;
        }
        
        this->target = this->team;
        this->state = STATE_RUN_HOME;
    }
    
    
    // Update position from velocity
    this->SetPosition(this->GetPosition() + this->velocity * dt);
    
    // Set angle
    float scale = 45/atan(1.0f);
    float angle = (scale*atan2(this->velocity.Y, this->velocity.X))-90;
    this->SetRotation(angle);
    
    // Wrap around world
    Vector2 tmp = this->GetPosition();
    if(tmp.X >= WORLD_WIDTH) {
        this->SetPosition(-WORLD_WIDTH, tmp.Y);
    } else if(tmp.X <= -WORLD_WIDTH) {
        this->SetPosition(WORLD_WIDTH, tmp.Y);
    }
    if(tmp.Y >= WORLD_HEIGHT) {
        this->SetPosition(tmp.X, -WORLD_HEIGHT);
    } else if(tmp.Y <= -WORLD_HEIGHT) {
        this->SetPosition(tmp.X, WORLD_HEIGHT);
    }
    
    // Show our current state
    if(Ant::DEBUG_DISPLAY) {
        std::stringstream state_str;
        std::string actual_state = "";
        
        if(this->state == STATE_SEEK_FOOD) {
            actual_state = "SEEK_FOOD";
        } else if(this->state == STATE_ATTACK) {
            actual_state = "ATTACK";
        } else if(this->state == STATE_FOLLOW_TRAIL) {
            actual_state = "FOLLOW_TRAIL";
        } else if(this->state == STATE_HELP_ANT) {
            actual_state = "HELP_ANT";
        } else if(this->state == STATE_RETURN_FOOD) {
            actual_state = "RETURN_FOOD";
        } else if(this->state == STATE_RUN_HOME) {
            actual_state = "RUN_HOME";
        }
        
        state_str << "State: " << actual_state;
        this->my_state->SetDisplayString(state_str.str());
        this->my_state->SetPosition(this->GetPosition());
        this->my_state->SetColor(1, 1, 1);
    } else {
        this->my_state->SetColor(1, 1, 1, 0);
    }
    
    // Bring food with us if we're carrying it
    if(this->carrying != NULL) {
        this->carrying->SetPosition(this->GetHeadPosition());
    }
}
 
/**
 * The ant does a random walk whilst looking for food
 */
void Ant::seekFood() {
    if(this->carrying != NULL) {
        this->target = this->team;
        this->state = STATE_RETURN_FOOD;
    }
    
    if(!this->_seeking_food) {
        if(this->velocity != NULL) {
            // Move roughly in the same dirction as we previously were
            this->velocity.Normalize();
            this->velocity = Vector2::Rotate(this->velocity, rand_range(-0.25f * 3.14f, 0.25f * 3.14f));
        } else {
            // Otherwise just choose a random direction
            this->velocity = Vector2::Rotate(Vector2::Vector2(1.0f, 0.0f), rand_range(0, 2*3.14f));
        }
        this->velocity *= ANT_SPEED;
        
        this->_seeking_food = true;
        theSwitchboard.DeferredBroadcast(new TypedMessage<Ant *>("AntChangeDirection", this), rand_range(0.15f, 4.0f));
    }
}

/**
 * Returns food to the nest, whilst leaving a trail
 */
void Ant::returnFood() {
    if(this->carrying == NULL || this->target == NULL) {
        // This will only happen if something odd happens
        // (the ant dies whilst about to drop it's food on the nest)
        this->target = NULL;
        this->state = STATE_SEEK_FOOD;
    }
    
    // Make a bee-line for the nest
    Vector2 heading = Vector2::Vector2(this->team->GetPosition());
    heading -= this->GetPosition();
    
    if(heading.Length() < NEST_DROP_RANGE * 0.5f) {
        this->carrying->being_carried = false;
        this->carrying = NULL;
        this->target = NULL;
        this->state = STATE_SEEK_FOOD;
        
        this->addXP(10);
    }
    
    heading.Normalize();
    heading *= ANT_SPEED * 0.5f;
    
    this->velocity = heading;
}

/**
 * Make a bee-line for the nest
 */
void Ant::runHome() {
    if(this->carrying != NULL) {
        this->carrying->being_carried = false;
        this->carrying = NULL;
    }
    
    // Make a bee-line for the nest
    Vector2 heading = Vector2::Vector2(this->team->GetPosition());
    heading -= this->GetPosition();
    
    if(heading.Length() < NEST_DROP_RANGE * 0.5f) {
        this->target = NULL;
        this->state = STATE_SEEK_FOOD;
    }
    
    heading.Normalize();
    heading *= ANT_SPEED * 0.75f;
    
    this->velocity = heading;
}

/**
 * Moves towards and attacks the current target
 */
void Ant::attackTarget() {
    
    // Make a bee-line for the target
    Vector2 heading = Vector2::Vector2(this->target->GetPosition());
    heading -= this->GetPosition();
    
    // If the target leaves our sight, or is picked up, or dies, forget about it
    float dist = heading.Length();
    if(dist > SIGHT_RANGE || ((Food *) this->target)->being_carried || ((Ant *) this->target)->dead) {
        this->target = NULL;
        this->state = STATE_SEEK_FOOD;
        return;
    }
    
    if(dist <= BITE_RANGE && this->_can_bite) {
        this->target->bitten();
        this->_can_bite = false;
        theSwitchboard.DeferredBroadcast(new TypedMessage<Ant *>("AntCanBite", this), BITE_DELAY);
        
        Food *food = (Food *) this->target;
        if(food != NULL) {
            food->being_carried = true;
            this->carrying = food;
            this->target = this->team;
            this->state = STATE_RETURN_FOOD;
        }
        
        // If we killed them, move on
        Ant *ant = (Ant *) this->target;
        if(ant != NULL && ant->dead) {
            this->target = NULL;
            this->state = STATE_SEEK_FOOD;
            this->addXP(20);
        }
    }
    
    heading.Normalize();
    heading *= ANT_SPEED;
    
    this->velocity = heading;
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
    this->experience += points;
    
    if(this->experience >= 100) {
        this->levelUp();
    }
}

void Ant::levelUp() {
    
    if(this->level == LEVEL_0) {
        this->level = LEVEL_1;
        this->SetSize(BASE_ANT_SIZE * 1.3f);
    } else if(this->level == LEVEL_1) {
        this->level = LEVEL_2;
        this->SetSize(BASE_ANT_SIZE * 1.6f);
    } else if(this->level == LEVEL_2) {
        this->level = LEVEL_3;
        this->SetSize(BASE_ANT_SIZE * 1.9f);
    } else if(this->level == LEVEL_3) {
        this->level = LEVEL_4;
        this->SetSize(BASE_ANT_SIZE * 2.1f);
    } else if(this->level == LEVEL_4) {
        this->level = LEVEL_5;
        this->SetSize(BASE_ANT_SIZE * 2.4f);
    } else if(this->level == LEVEL_5) {
        this->level = LEVEL_6;
        this->SetSize(BASE_ANT_SIZE * 2.7f);
    } else if(this->level == LEVEL_6) {
        this->level = LEVEL_7;
        this->SetSize(BASE_ANT_SIZE * 3.0f);
    } else {
        return;
    }
    
    this->health += BITE_HURT;
    this->health_max += BITE_HURT;
    this->experience = experience % 100;
    
}

/**
 * Handle incoming messages
 */
void Ant::ReceiveMessage(Message *msg) {
    
    if(msg->GetMessageName() == "AntChangeDirection") {
        TypedMessage<Ant *> *tmp = (TypedMessage<Ant *> *) msg;
        if(tmp != NULL && tmp->GetValue() == this) {
            this->_seeking_food = false;
        }
    } else if(msg->GetMessageName() == "AntCanBite") {
        TypedMessage<Ant *> *tmp = (TypedMessage<Ant *> *) msg;
        if(tmp != NULL && tmp->GetValue() == this) {
            this->_can_bite = true;
        }
    } else if(msg->GetMessageName() == "F1Pressed") {
        Ant::DEBUG_DISPLAY = !Ant::DEBUG_DISPLAY;
    }
}

void Ant::bitten() {
    this->health -= BITE_HURT;
    
    if(this->health <= 0 && !this->dead) {
        this->dead = true;
    } else {
        this->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        this->ChangeColorTo(this->team->color, 0.75f, true);
    }
}

Vector2 Ant::GetHeadPosition() {
    return this->GetPosition() + Vector2::Rotate(Vector2::Vector2(ANT_HEAD_LENGTH, 0), (this->GetRotation() + 90) * 3.14f / 180);
}

void Ant::die() {
    theGame.ForceAddFoodBit(this->GetPosition().X, this->GetPosition().Y);
    
    if(this->carrying != NULL) {
        this->carrying->being_carried = false;
        this->carrying = NULL;
    }
    
    int pos = 0;
    for(std::vector<Ant *>::iterator i=this->team->ants.begin(); i!=this->team->ants.end(); i++) {
        if((*i) == this) {
            break;
        }
        pos++;
    }
    
    theWorld.Remove(this->my_state);
    theWorld.Remove(this);
    this->team->ants.erase(this->team->ants.begin() + pos);
    
}

void Ant::dropFood() {
    if(this->carrying != NULL) {
        this->carrying->being_carried = false;
        this->carrying = NULL;
    }
}



