
#ifndef ANTGAME_SCENE_MAIN
#define ANTGAME_SCENE_MAIN

#include "stdafx.h"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "scene_manager.h"
#include "game.h"
#include "color.h"
#include "ant.h"
#include "food.h"


class SceneMain : public Scene, public MessageListener, public MouseListener {
public:
	SceneMain();

	virtual void Start();
	virtual void Update(float dt);
    virtual void Render();

private:
    Game *game;
    
	virtual void ReceiveMessage(Message *message);
};


#endif
