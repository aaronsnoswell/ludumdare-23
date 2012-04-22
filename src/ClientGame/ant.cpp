
#include "ant.h"

#include "ant_colony.h"
#include "food.h"
#include "player.h"

Ant::Ant(AntColony *team, float x, float y) {
    ANT_SPEED = rand_range(2.0f, 3.0f);
    ANT_HEALTH = 10;
    
    this->team = team;
    this->SetPosition(x, y);
    this->SetColor(team->color);
    this->SetSize(1.8f);
    this->SetSprite("Resources/Images/ant.png");
    
    this->level = LEVEL_0;
    this->points = 0;
    this->state = STATE_SEEK_FOOD;
    this->target = NULL;
    this->carrying = NULL;
    this->dead = false;
    this->health = ANT_HEALTH;
    
    this->_seeking_food = false;
    
    theSwitchboard.SubscribeTo(this, "AntChangeDirection");
    theSwitchboard.SubscribeTo(this, "AntFadeOut");
    theSwitchboard.SubscribeTo(this, "AntFadeIn");
}


void Ant::Update(float dt) {
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
    /*
    AntColony *enemy_team = (theGame.blue_colony == this->team) ? theGame.red_colony : theGame.blue_colony;
    if(this->target == NULL) {
        for(std::vector<Ant *>::iterator i=enemy_team->ants.begin(); i!=enemy_team->ants.end(); i++) {
            if(dist_check(this->GetPosition(), (*i)->GetPosition(), SIGHT_RANGE)) {
                // Go attack the ant
                this->target = (*i);
                this->dropFood();
                this->state = STATE_ATTACK;
            }
        }
    }
     */
    
    // Check for food
    /*
    if(this->target == NULL && this->carrying == NULL) {
        for(std::vector<Food *>::iterator i=theGame.foodbits.begin(); i!=theGame.foodbits.end(); i++) {
            if(dist_check(this->GetPosition(), (*i)->GetPosition(), SIGHT_RANGE) && ((*i)->carriedBy == NULL)) {
                // Go eat the food
                this->target = (*i);
                this->state = STATE_ATTACK;
            }
        }
    }
    */
    
    
    // Update position from velocity
    this->SetPosition(this->GetPosition() + this->velocity * dt);
    
    // Set angle
    float scale = 45/atan(1);
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
    
}
 
/**
 * The ant does a random walk whilst looking for food
 */
void Ant::seekFood() {
    if(!this->_seeking_food) {
        if(this->velocity != NULL) {
            // Move roughly in the same dirction as we previously were
            this->velocity.Normalize();
            this->velocity = Vector2::Rotate(this->velocity, rand_range(-0.25f * M_PI, 0.25f * M_PI));
        } else {
            // Otherwise just choose a random direction
            this->velocity = Vector2::Rotate(Vector2::Vector2(1.0f, 0.0f), rand_range(0, 2*M_PI));
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
    
    
    
    /*
    // Make a bee-line for the nest
    Vector2 heading = Vector2::Vector2(this->team->GetPosition());
    heading -= this->GetPosition();
    
    if(heading.Length() < BITE_RANGE) {
        this->team->addFood(this->carrying);
        this->carrying = NULL;
        this->target = NULL;
        this->state = STATE_SEEK_FOOD;
    }
    
    heading.Normalize();
    heading *= ANT_SPEED * 0.5f;
    
    this->velocity = heading;
     */
}

/**
 * Moves towards and attacks the current target
 */
void Ant::attackTarget() {
    
    
    
    /*
    // Make a bee-line for the target
    Vector2 heading = Vector2::Vector2(this->target->GetPosition());
    heading -= this->GetPosition();
    
    if(heading.Length() > SIGHT_RANGE) {
        this->target = NULL;
        this->state = STATE_SEEK_FOOD;
        return;
    }
    
    heading.Normalize();
    heading *= ANT_SPEED;
    
    this->velocity = heading;
    
    if(Vector2::Vector2(this->GetPosition() - this->target->GetPosition()).Length() < BITE_RANGE) {
        this->target->bite(this);
        
        Food *tmp = dynamic_cast<Food *>(this->target);
        Ant *tmp2 = dynamic_cast<Ant *>(this->target);
        if(tmp != NULL) {
            tmp->carriedBy = this;
            this->carrying = tmp;
            this->target = NULL;
            this->state = STATE_RETURN_FOOD;
        } else if(tmp2 != NULL) {
            if(tmp2->dead) {
                this->target = NULL;
                this->state = STATE_SEEK_FOOD;
            }
        }
    }
     */
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
    
    this->health += BITE_HURT;
    this->points = points % 100;
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
    } else if(msg->GetMessageName() == "AntFadeOut") {
        TypedMessage<Ant *> *tmp = (TypedMessage<Ant *> *) msg;
        if(tmp != NULL && tmp->GetValue() == this) {
            this->ChangeColorTo(((MyColor) this->GetColor()).setA(0), 0.1f);
            theSwitchboard.DeferredBroadcast(new TypedMessage<Ant *>("AntFadeIn", this), 0.1f);
        }
    } else if(msg->GetMessageName() == "AntFadeIn") {
        TypedMessage<Ant *> *tmp = (TypedMessage<Ant *> *) msg;
        if(tmp != NULL && tmp->GetValue() == this) {
            this->ChangeColorTo(this->team->GetColor(), 0.1f);
        }
    }
}

void Ant::bitten() {
    this->health -= BITE_HURT;
    
    if(this->health <= 0 && !this->dead) {
        this->dead = true;
    } else {
        //theSwitchboard.Broadcast(new TypedMessage<Ant *>("AntFadeOut", this));
        //this->ChangeColorTo(Color::Color(0,0,0,1), 0.1f);
    }
}

Vector2 Ant::GetHeadPosition() {
    return this->GetPosition() + Vector2::Rotate(Vector2::Vector2(ANT_HEAD_LENGTH, 0), (this->GetRotation() + 90) * M_PI / 180);
}

void Ant::die() {
    theGame.ForceAddFoodBit(this->GetPosition().X, this->GetPosition().Y);
    
    int pos = 0;
    for(std::vector<Ant *>::iterator i=this->team->ants.begin(); i!=this->team->ants.end(); i++) {
        if((*i) == this) {
            break;
        }
        pos++;
    }
    
    theWorld.Remove(this);
    this->team->ants.erase(this->team->ants.begin() + pos);
    
}

void Ant::dropFood() {
    if(this->carrying != NULL) {
        this->carrying->being_carried = false;
        this->carrying = NULL;
    }
}



