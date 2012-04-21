

#include "stdafx.h"

#include "game_manager.h"

#include "screen_main.h"

Screen::Screen() {}

void Screen::Start() {}

void Screen::Stop() {
    std::vector<Renderable*>::iterator it = _objects.begin();
    while(_objects.end() != it) {
        (*it)->Destroy();
        it++;
    }
    _objects.clear();
}

void Screen::Update(float dt) {}
void Screen::Render() {}


MyGameManager* MyGameManager::s_MyGameManager = NULL;

MyGameManager::MyGameManager() {
    // Add the game's screens
    _screens.push_back(new ScreenMain());
    
    unsigned int startingIndex = 0;
    if (_screens.size() > startingIndex) {
        theWorld.Add(_screens[startingIndex]);
        _screens[startingIndex]->Start();
        _current = startingIndex;
    } else {
        _current = -1;
    }
}

MyGameManager& MyGameManager::GetInstance() {
    if (s_MyGameManager == NULL) {
        s_MyGameManager = new MyGameManager();
    }
    return *s_MyGameManager;
}

Screen* MyGameManager::GetCurrentScreen() {
    return _screens[_current];
}

void MyGameManager::ChangeScreen(int screen) {
    _screens[_current]->Stop();
    theWorld.Remove(_screens[_current]);
    _screens[screen]->Start();
    theWorld.Add(_screens[screen]);
}

void MyGameManager::Render() {
    glColor3f(0.5f, 0.5f, 0.5f);
    DrawGameText("Hello, World!", "ConsoleSmall", 0, 0);
}



