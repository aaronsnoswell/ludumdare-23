
#include "game.h"

#include "ant_colony.h"

Game *Game::_Game = NULL;

Game::Game() {
    // Create the ground
    for(int i=0; i<GROUND_TILES; i++) {
        Actor * tmp = new Actor();
        tmp->SetColor(((MyColor) Color::FromHexString(GROUND_COLOR)).mutate(0.03f));
        tmp->SetPosition(
                         (1.0f * rand() / RAND_MAX * WORLD_WIDTH) - (WORLD_WIDTH / 2.0f),
                         (1.0f * rand() / RAND_MAX * WORLD_HEIGHT) - (WORLD_HEIGHT / 2.0f)
                         );
        theWorld.Add(tmp);
        ground_tiles.push_back(tmp);
    }
    
    ground = new FullScreenActor();
    ground->SetColor(Color::FromHexString(GROUND_COLOR));
    theWorld.Add(ground, -2);
    
    
    // Seed the world with some food
    /*
    for(int i=0; i<FOOD_PEICES; i++) {
        Food *tmp = new Food(
                             rand_range(-0.5f * WORLD_WIDTH, 0.5f * WORLD_WIDTH),
                             rand_range(-0.5f * WORLD_HEIGHT, 0.5f * WORLD_HEIGHT)
                             );
        theWorld.Add(tmp);
        
        foodbits.push_back(tmp);
    }
     */
    
    
    // Create the Time-Of-Day actor
    time_shade = new FullScreenActor();
    time_shade->SetColor(((MyColor) Color::FromHexString("#070922")).setA(0.0f));
    theWorld.Add(time_shade);
    
    
    
    AntColony *col = new AntColony("Blue", "#1111ff", -3, 3);
    for(int i=0; i<COLONY_START_SIZE; i++) {
        Ant *tmp = new Ant(
                           col,
                           ((2.0f * rand() / RAND_MAX) - 1.0f) + col->GetPosition().X,
                           ((2.0f * rand() / RAND_MAX) - 1.0f) + col->GetPosition().Y
                           );
        theWorld.Add(tmp);
        ants.push_back(tmp);
    }
    
    
    
    
    // Subscribe to messages
    theSwitchboard.SubscribeTo(this, "TIME_MIDDAY");
    theSwitchboard.SubscribeTo(this, "TIME_MIDNIGHT");
}

Game& Game::GetInstance() {
    if (_Game == NULL) {
        _Game = new Game();
    }
    return *_Game;
}

void Game::Start(std::vector<Renderable *> objects) {
    for(std::vector<Actor *>::iterator i=ground_tiles.begin(); i!=ground_tiles.end(); i++) {
        objects.push_back(*i);
    }
    objects.push_back(ground);
    objects.push_back(time_shade);
    
    for(std::vector<Ant *>::iterator i=ants.begin(); i!=ants.end(); i++) {
        objects.push_back(*i);
    }
    
    // Start the day-night cycle
    theSwitchboard.Broadcast(new Message("TIME_MIDDAY"));
}

void Game::Update(float dt) {
    // Update all ants
    for(std::vector<Ant *>::iterator i=ants.begin(); i!=ants.end(); i++) {
        (*i)->update(dt);
    }
}

void Game::ReceiveMessage(Message *message) {
    if(message->GetMessageName() == "TIME_MIDDAY") {
        time_shade->ChangeColorTo(((MyColor) time_shade->GetColor()).setA(0.5f), DAY_CYCLE_LENGTH, true, "TIME_MIDNIGHT");
    } else if(message->GetMessageName() == "TIME_MIDNIGHT") {
        time_shade->ChangeColorTo(((MyColor) time_shade->GetColor()).setA(0.0f), DAY_CYCLE_LENGTH, true, "TIME_MIDDAY");
    }
}


