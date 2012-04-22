
#include "scene_main.h"

#include "scene_manager.h"

Scene::Scene() {}

void Scene::Start() {}

void Scene::Stop() {
    std::vector<Renderable*>::iterator it = _objects.begin();
    while(_objects.end() != it) {
        (*it)->Destroy();
        it++;
    }
    _objects.clear();
}

void Scene::Update(float dt) {}
void Scene::Render() {}


SceneManager* SceneManager::s_SceneManager = NULL;

SceneManager::SceneManager() {
    // Add the game's screens
    _scenes.push_back(new SceneMain());
    
    unsigned int startingIndex = 0;
    if (_scenes.size() > startingIndex) {
        theWorld.Add(_scenes[startingIndex]);
        _scenes[startingIndex]->Start();
        _current = startingIndex;
    } else {
        _current = -1;
    }
}

SceneManager& SceneManager::GetInstance() {
    if (s_SceneManager == NULL) {
        s_SceneManager = new SceneManager();
    }
    return *s_SceneManager;
}

Scene* SceneManager::GetCurrentScene() {
    return _scenes[_current];
}

void SceneManager::ChangeScene(int scene) {
    _scenes[_current]->Stop();
    theWorld.Remove(_scenes[_current]);
    _scenes[scene]->Start();
    theWorld.Add(_scenes[scene]);
}

void SceneManager::Render() {
    // Pass
}



