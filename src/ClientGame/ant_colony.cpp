
#include "ant_colony.h"

#include "food.h"

AntColony::AntColony(std::string name, std::string color, float x, float y) {
    this->name = name;
    this->color = Color::FromHexString(color);
    this->SetPosition(x, y);
    this->food_store = FOOD_START;
    
    
    this->SetColor(this->color);
    this->SetSize(6.0f);
    this->SetSprite("Resources/Images/anthill.png");
    
    theSwitchboard.SubscribeTo(this, "SpawnAnt");
    
    theSwitchboard.DeferredBroadcast(new TypedMessage<std::string>("SpawnAnt", this->name), ANT_SPAWN_TIME);
    this->_spawning = true;
}

void AntColony::addFood(Food *f) {
    this->food_store += f->value;
    f->consumed = true;
    
    if(this->food_store >= ANT_COST && !this->_spawning) {
        theSwitchboard.DeferredBroadcast(new TypedMessage<std::string>("SpawnAnt", this->name), ANT_SPAWN_TIME);
        this->_spawning = true;
    }
}

void AntColony::Update(float dt) {
    // Update all ants
    std::vector<Ant *> to_remove;
    for(std::vector<Ant *>::iterator i=ants.begin(); i!=ants.end(); i++) {
        if((*i)->dead) {
            to_remove.push_back(*i);
        } else {
            (*i)->Update(dt);
        }
    }
    
    // Remove any dead ants
    for(int i=0; i<to_remove.size(); i++) {
        to_remove[i]->die();
    }
    
    // Check if any food has been dropped on the nest entrance
    for(std::vector<Food *>::iterator i=theGame.foodbits.begin(); i!=theGame.foodbits.end(); i++) {
        if(!((*i)->being_carried)) {
            if(dist_check(this->GetPosition(), (*i)->GetPosition(), NEST_DROP_RANGE)) {
                this->addFood(*i);
            }
        }
    }
    
}

void AntColony::SpawnAnt() {
    this->food_store -= ANT_COST;
    Ant *tmp = new Ant(this, this->GetPosition().X, this->GetPosition().Y);
    ants.push_back(tmp);
    theWorld.Add(tmp, LAYER_ANT);
    
    if(this->food_store >= ANT_COST && !this->_spawning) {
        theSwitchboard.DeferredBroadcast(new TypedMessage<std::string>("SpawnAnt", this->name), ANT_SPAWN_TIME);
        this->_spawning = true;
    }
}

/**
 * Spawns an ant with disregard for the timeout and food requirements
 */
void AntColony::ForceSpawnAnt() {
    Ant *tmp = new Ant(this, this->GetPosition().X, this->GetPosition().Y);
    ants.push_back(tmp);
    theWorld.Add(tmp);
}

void AntColony::ReceiveMessage(Message *m) {
    if(m->GetMessageName() == "SpawnAnt") {
        TypedMessage<std::string> *tmp = (TypedMessage<std::string> *) m;
        if(tmp != NULL && ((std::string) tmp->GetValue()) == this->name) {
            this->_spawning = false;
            this->SpawnAnt();
        }
    }
}


