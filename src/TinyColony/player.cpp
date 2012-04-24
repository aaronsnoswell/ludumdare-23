
#include "stdafx.h"

#include "player.h"

#include "ant_colony.h"
#include "food.h"

Player::Player(AntColony *team, float x, float y) : Ant(team, x, y) {
    
    ANT_SPEED = 3.0f;
    
    SPACE_PRESSED = LEFT_PRESSED = RIGHT_PRESSED = UP_PRESSED = DOWN_PRESSED = false;
    
    
    bite = theSound.LoadSample("Resources/Sounds/bite.wav", false);
    hurt = theSound.LoadSample("Resources/Sounds/hurt.wav", false);
    xp = theSound.LoadSample("Resources/Sounds/xp.wav", false);
    levelup = theSound.LoadSample("Resources/Sounds/levelup.wav", false);
    
    // Subscribe to events
    theSwitchboard.SubscribeTo(this, "LeftPressed");
    theSwitchboard.SubscribeTo(this, "RightPressed");
    theSwitchboard.SubscribeTo(this, "UpPressed");
    theSwitchboard.SubscribeTo(this, "DownPressed");
    theSwitchboard.SubscribeTo(this, "LeftReleased");
    theSwitchboard.SubscribeTo(this, "RightReleased");
    theSwitchboard.SubscribeTo(this, "UpReleased");
    theSwitchboard.SubscribeTo(this, "DownReleased");
    
    theSwitchboard.SubscribeTo(this, "SpacePressed");
    theSwitchboard.SubscribeTo(this, "SpaceReleased");
}


void Player::Update(float dt) {
    Actor::Update(dt);
    
    if(this->dead) {
        this->dead = false;
        this->SetPosition(this->team->GetPosition());
        this->SetSize(BASE_ANT_SIZE);
    }
    
    float x = 0, y = 0;
    
    if(LEFT_PRESSED) {
        x += -ANT_SPEED;
    }
    if(RIGHT_PRESSED) {
        x += ANT_SPEED;
    }
    if(UP_PRESSED) {
        y += ANT_SPEED;
    }
    if(DOWN_PRESSED) {
        y += -ANT_SPEED;
    }
    
    this->velocity = Vector2::Vector2(x, y);
    
    if(SPACE_PRESSED && this->carrying != NULL) {
        this->carrying->SetPosition(this->GetHeadPosition());
        this->velocity *= 0.5f;
    }
    
    this->SetPosition(this->GetPosition() + this->velocity * dt);
    theCamera.SetPosition(this->GetPosition());
    
    // Set angle
    if(x != 0 || y != 0) {
        float scale = 45/atan(1.0f);
        float angle = (scale*atan2(this->velocity.Y, this->velocity.X))-90;
        this->SetRotation(angle);
    }
    
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


void Player::ReceiveMessage(Message *m) {
    if(m->GetMessageName() == "LeftPressed") {
        LEFT_PRESSED = true;
    } else if(m->GetMessageName() == "RightPressed") {
        RIGHT_PRESSED = true;
    } else if(m->GetMessageName() == "UpPressed") {
        UP_PRESSED = true;
    } else if(m->GetMessageName() == "DownPressed") {
        DOWN_PRESSED = true;
    } else if(m->GetMessageName() == "LeftReleased") {
        LEFT_PRESSED = false;
    } else if(m->GetMessageName() == "RightReleased") {
        RIGHT_PRESSED = false;
    } else if(m->GetMessageName() == "UpReleased") {
        UP_PRESSED = false;
    } else if(m->GetMessageName() == "DownReleased") {
        DOWN_PRESSED = false;
    } else if(m->GetMessageName() == "SpacePressed") {
        SPACE_PRESSED = true;
        // Cache head position
        Vector2 head_pos = this->GetHeadPosition();
        
        theSound.PlaySound(bite);
        
        // Bite any nearby enemy ant
        for(std::vector<Ant *>::iterator i=theGame.red_colony->ants.begin(); i!=theGame.red_colony->ants.end(); i++) {
            if(dist_check(head_pos, (*i)->GetPosition(), BITE_RANGE)) {
                (*i)->bitten();
                
                if((*i)->dead) {
                    this->addXP(20);
                }
            }
        }
        
        // Grab onto any nearby food bit
		// XXX ajs VC++'s picky compiler settings complain about shortest_dist not being defined...
        float shortest_dist = 999999.0f;
        Food *closest = NULL;
        for(std::vector<Food *>::iterator i=theGame.foodbits.begin(); i!=theGame.foodbits.end(); i++) {
            if(!(*i)->being_carried) {
                float length = Vector2::Vector2(head_pos - (*i)->GetPosition()).Length();
                
                if(dist_check(head_pos, (*i)->GetPosition(), BITE_RANGE)) {
                    if(length < shortest_dist || closest == NULL) {
                        closest = *i;
                        shortest_dist = length;
                    }
                }
            }
        }
        if (closest != NULL) {
            this->carrying = closest;
            this->carrying->being_carried = true;
        }
        
        
    } else if(m->GetMessageName() == "SpaceReleased") {
        SPACE_PRESSED = false;
        
        // Release the foodbit
        if(this->carrying != NULL) {
            this->carrying->being_carried = false;
            
            if(dist_check(this->carrying->GetPosition(), this->team->GetPosition(), NEST_DROP_RANGE)) {
                this->addXP(10);
            }
            
        }
        this->carrying = NULL;
        
    }
}

void Player::addXP(int points) {
    Ant::addXP(points);
    theSound.PlaySound(xp);
}

void Player::levelUp() {
    Ant::levelUp();
    theSound.PlaySound(levelup);
}

void Player::bitten() {
    this->health -= BITE_HURT;
    theSound.PlaySound(hurt);
    
    if(this->health <= 0) {
        this->die();
    } else {
        this->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        this->ChangeColorTo(this->team->color, 0.75f, true);
    }
}

void Player::die() {
    this->dead = true;
    theGame.ForceAddFoodBit(this->GetPosition().X, this->GetPosition().Y);
    
    if(this->carrying != NULL) {
        this->carrying->being_carried = false;
        this->carrying = NULL;
    }
    
    this->health = ANT_HEALTH;
    this->experience = 0;
    this->level = LEVEL_0;
    
    // Player position is updated in the Update function
}





