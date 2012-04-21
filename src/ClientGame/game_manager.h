#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

class Screen : public Renderable {
public:
	Screen();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float dt);
	virtual void Render();
	
	virtual void ReceiveMessage(Message *message) {}

protected:
	std::vector<Renderable*> _objects;
};


#define theGame MyGameManager::GetInstance()

class MyGameManager : public GameManager {
public:

	static MyGameManager &GetInstance();
	static const int SCREEN_MENU = 0;
	static const int SCREEN_MAIN = 0;

	Screen* GetCurrentScreen();
	void ChangeScreen(int screen);
    void Render();

protected:
	MyGameManager();
	static MyGameManager *s_MyGameManager;

private:
	std::vector<Screen*> _screens;
	int _current;
};

