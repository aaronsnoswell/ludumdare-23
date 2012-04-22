
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
    virtual void MouseMotionEvent(Vec2i screen_coords);
    virtual void MouseDownEvent(Vec2i screen_coords, MouseButtonInput button);
    virtual void MouseUpEvent(Vec2i screen_coords, MouseButtonInput button);
    virtual void MouseWheelEvent(int position);
    int _mouse_wheel_pos;
    
    Vector2 LMB_DOWN, MMB_DOWN, RMB_DOWN;
};


#endif
