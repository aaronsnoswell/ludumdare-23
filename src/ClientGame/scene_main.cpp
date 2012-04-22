

#include "scene_main.h"

#include "ant_colony.h"

#include <sstream>

SceneMain::SceneMain() {
    // Randomly generate the world
    game = &theGame;
    
    // Load the background music
    AngelSampleHandle music = theSound.LoadSample("Resources/Sounds/xylophone.ogg", false);
    theSound.PlaySound(music);
    
    _mouse_wheel_pos = 0;
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
}

void SceneMain::ReceiveMessage(Message *message) {
    // Pass
}

void SceneMain::MouseMotionEvent(Vec2i screen_coords) {
    if(LMB_DOWN != NULL) {
        Vector2 *tmp = new Vector2(theCamera.GetPosition());
        Vector2 *tmp2 = new Vector2(LMB_DOWN.X - screen_coords.X, screen_coords.Y - LMB_DOWN.Y);
        *tmp2 *= 0.025f;
        
        *tmp += *tmp2;
        
        theCamera.SetPosition(*tmp);
        
    }
    
    if(LMB_DOWN != NULL) LMB_DOWN = screen_coords;
    if(MMB_DOWN != NULL) MMB_DOWN = screen_coords;
    if(RMB_DOWN != NULL) RMB_DOWN = screen_coords;
}

void SceneMain::MouseDownEvent(Vec2i screen_coords, MouseButtonInput button) {
    if(button == MOUSE_LEFT) {
        LMB_DOWN = screen_coords;
    } else if (button == MOUSE_MIDDLE) {
        MMB_DOWN = screen_coords;
    } else if (button == MOUSE_RIGHT) {
        RMB_DOWN = screen_coords;
    }
}

void SceneMain::MouseUpEvent(Vec2i screen_coords, MouseButtonInput button) {
    if(button == MOUSE_LEFT) {
        LMB_DOWN = NULL;
    } else if (button == MOUSE_MIDDLE) {
        MMB_DOWN = NULL;
    } else if (button == MOUSE_RIGHT) {
        RMB_DOWN = NULL;
    }
}

void SceneMain::MouseWheelEvent(int position) {
    int delta = _mouse_wheel_pos - position;
    _mouse_wheel_pos = position;
    
    theCamera.SetPosition(
        theCamera.GetPosition().X,
        theCamera.GetPosition().Y,
        std::max(1.0f, std::min(40.0f, theCamera.GetZ() + delta * 0.5f))
    );
}



