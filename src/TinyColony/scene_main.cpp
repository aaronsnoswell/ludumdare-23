

#include "scene_main.h"

#include "ant_colony.h"
#include "player.h"

#include <sstream>

SceneMain::SceneMain() {
    // Random seeds, please
    srand(time(NULL));
    
    // Randomly generate the world
    game = &theGame;
    
    // Load the background music
    AngelSampleHandle music = theSound.LoadSample("Resources/Sounds/xylophone.ogg", true);
    theSound.PlaySound(music, 0.5f, true);
}

void SceneMain::Start() {
    theGame.Start(_objects);
    
}

void SceneMain::Update(float dt) {
    theGame.Update(dt);
}

void SceneMain::Render() {
    glColor3f(1.0f, 1.0f, 1.0f);
    
    std::stringstream tmp;
    tmp << "Blue Food: " << theGame.blue_colony->food_store << " Pop: " << theGame.blue_colony->ants.size();
    DrawGameText(tmp.str(), "Console", 5, 24, 0);
    tmp.str("");
    tmp << "Red Food: " << theGame.red_colony->food_store << " Pop: " << theGame.red_colony->ants.size();
    DrawGameText(tmp.str(), "Console", 5, 24 + 24, 0);
    
    tmp.str("");
    tmp << "LVL: " << theGame.player->level;
    DrawGameText(tmp.str(), "Console", 5, theCamera.GetWindowHeight() - 5 - 24, 0);
    tmp.str("");
    tmp << "HP: " << theGame.player->health << "/" << theGame.player->health_max;
    tmp << " XP: " << theGame.player->experience << "/" << 100;
    DrawGameText(tmp.str(), "Console", 5, theCamera.GetWindowHeight() - 5, 0);
}

void SceneMain::ReceiveMessage(Message *message) {
    // Pass
}




