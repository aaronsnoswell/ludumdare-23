
#include "game.h"

#include "ant_colony.h"
#include "food.h"
#include "player.h"

Game *Game::_Game = NULL;

Game::Game() {
    // Create the world
    ground = new Actor();
    //ground->SetColor(Color::FromHexString(GROUND_COLOR));
    ground->SetSize(WORLD_WIDTH * 3);
    ground->SetSprite("Resources/Images/dirt.png", 0, GL_REPEAT);
    ground->SetColor(1, 1, 1, 1);
    ground->SetUVs(*(new Vector2(0, 0)), *(new Vector2(17.0f, 20.0f)));
    theWorld.Add(ground, LAYER_GROUND);
    
    // Seed the world with some food
    while(_food_locs.size() != FOOD_CACHES) {
        _food_locs.push_back(std::pair<Vector2 *, int>(new Vector2(rand_range(-WORLD_WIDTH + 5, WORLD_WIDTH - 5), rand_range(-WORLD_HEIGHT + 5, WORLD_HEIGHT - 5)), 0));
    }
    
    for(int i=FOOD_PEICES; i>0; i--) {
        int which = rand() % FOOD_CACHES;
        Vector2 *cache_location = _food_locs[which].first;
        
        Food *tmp = new Food(
                             gauss_rand(cache_location->X, 5.0f),
                             gauss_rand(cache_location->Y, 5.0f)
                             );
        
        theWorld.Add(tmp, LAYER_FOOD);
        foodbits.push_back(tmp);
        
        _food_locs[which].second++;
    }
    
    // Create the Time-Of-Day actor
    time_shade = new Actor();
    time_shade->SetColor(((MyColor) Color::FromHexString("#070922")).setA(0.0f));
    time_shade->SetSize(WORLD_WIDTH * 3);
    theWorld.Add(time_shade, LAYER_TIMESHADE);
    
    
    blue_colony = new AntColony("Blue", "#1111ff", rand_range(-WORLD_WIDTH, WORLD_WIDTH), rand_range(-WORLD_HEIGHT, WORLD_HEIGHT));
    theWorld.Add(blue_colony, LAYER_NEST);
    
    red_colony = new AntColony("Red", "#ff1111", rand_range(-WORLD_WIDTH, WORLD_WIDTH), rand_range(-WORLD_HEIGHT, WORLD_HEIGHT));
    theWorld.Add(red_colony, LAYER_NEST);
    
    
    instructions = new TextActor("Console", "Aloha! Thanks for trying my game\nYou are a tiny ant in a tiny world.\nThis is your colony. Your goal is to defend it,\nand defeat the red ants.\n\nUse the arrow keys or WASD to move, SPACE to bite.\nCollect food (   ) and kill red ants to level up!\n\nMost importantly, Have fun!\n\n~Aaron Snoswell (Enter dismisses this message)");
    instructions->SetPosition(blue_colony->GetPosition() + Vector2::Vector2(3.5f, 2));
    instructions->SetColor(1, 1, 1);
    theWorld.Add(instructions);
    
    this->ForceAddFoodBit(blue_colony->GetPosition().X + 8.45f, blue_colony->GetPosition().Y - 1.9f);
    
    player = new Player(blue_colony, blue_colony->GetPosition().X, blue_colony->GetPosition().Y);
    blue_colony->ants.push_back(player);
    theWorld.Add(player, LAYER_ANT);
    
    // Add an ant to the reds to even things out
    red_colony->ForceSpawnAnt();
    
    home_arrow = new HUDActor();
    home_arrow->SetPosition(theCamera.GetWindowWidth() - 60, theCamera.GetWindowHeight() - 60);
    home_arrow->SetSize(100);
    home_arrow->SetSprite("Resources/Images/arrow.png");
    home_arrow->SetColor(blue_colony->color);
    theWorld.Add(home_arrow, LAYER_TIMESHADE + 1);
    
    // Subscribe to messages
    theSwitchboard.SubscribeTo(this, "TIME_MIDDAY");
    theSwitchboard.SubscribeTo(this, "TIME_MIDNIGHT");
    theSwitchboard.SubscribeTo(this, "EnterPressed");
}

Game& Game::GetInstance() {
    if (_Game == NULL) {
        _Game = new Game();
    }
    return *_Game;
}

void Game::Start(std::vector<Renderable *> objects) {
    objects.push_back(ground);
    objects.push_back(time_shade);
    
    objects.push_back(blue_colony);
    for(std::vector<Ant *>::iterator i=blue_colony->ants.begin(); i!=blue_colony->ants.end(); i++) {
        objects.push_back(*i);
    }
    
    objects.push_back(red_colony);
    for(std::vector<Ant *>::iterator i=red_colony->ants.begin(); i!=red_colony->ants.end(); i++) {
        objects.push_back(*i);
    }
    
    // Start the day-night cycle
    theSwitchboard.Broadcast(new Message("TIME_MIDDAY"));
}

void Game::Update(float dt) {
    // The player is updated via the blue colony
    blue_colony->Update(dt);
    red_colony->Update(dt);
    
    // Check for any foodbits that are pending removal
    std::vector<Food *> consumed_food;
    for(std::vector<Food *>::iterator i=foodbits.begin(); i!=foodbits.end(); i++) {
        if((*i)->consumed) {
            consumed_food.push_back(*i);
        }
    }
    for(int i=0; i<consumed_food.size(); i++) {
        consumed_food[i]->Remove();
    }
    
    if(red_colony->ants.size() == 0) {
        instructions->SetDisplayString("Victory! The red ant amry has been crushed!");
        instructions->SetPosition(player->GetPosition());
        instructions->SetColor(1, 1, 1);
    }
    
    // Update the radar arrow
    Vector2 delta = Vector2::Vector2(player->GetPosition() - blue_colony->GetPosition());
    float scale = 45/atan(1);
    float angle = (scale*atan2(delta.Y, delta.X))+90;
    home_arrow->SetRotation(angle);
}

void Game::ReceiveMessage(Message *message) {
    if(message->GetMessageName() == "TIME_MIDDAY") {
        time_shade->ChangeColorTo(((MyColor) time_shade->GetColor()).setA(0.5f), DAY_CYCLE_LENGTH, true, "TIME_MIDNIGHT");
    } else if(message->GetMessageName() == "TIME_MIDNIGHT") {
        time_shade->ChangeColorTo(((MyColor) time_shade->GetColor()).setA(0.0f), DAY_CYCLE_LENGTH, true, "TIME_MIDDAY");
    } else if(message->GetMessageName() == "EnterPressed") {
        instructions->ChangeColorTo(Color::Color(1,1,1,0), 1.0f);
    }
}

void Game::ForceAddFoodBit(float x, float y) {
    Food *tmp = new Food(x, y);
    theWorld.Add(tmp, LAYER_FOOD);
    foodbits.push_back(tmp);
}



