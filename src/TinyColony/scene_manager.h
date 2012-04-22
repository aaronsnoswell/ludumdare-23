
#ifndef ANTGAME_SCENE_MANAGER_H
#define ANTGAME_SCENE_MANAGER_H

#include "stdafx.h"

#include <vector>
#include <string>

#include "color.h"

class Scene : public Renderable {
public:
	Scene();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float dt);
	virtual void Render();
	
	virtual void ReceiveMessage(Message *message) {}

protected:
	std::vector<Renderable*> _objects;
};


#define theSceneManager SceneManager::GetInstance()

class SceneManager : public GameManager {
public:

	static SceneManager &GetInstance();
	static const int SCENE_MENU = 0;
	static const int SCENE_MAIN = 0;

	Scene* GetCurrentScene();
	void ChangeScene(int scene);
    void Render();

protected:
	SceneManager();
	static SceneManager *s_SceneManager;

private:
	std::vector<Scene*> _scenes;
	int _current;
};

#endif

