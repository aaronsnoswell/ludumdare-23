
#pragma once

#include <vector>
#include <cstdlib>
#include <cmath>

#include "game_manager.h"

class ScreenMain : public Screen, public MessageListener {
public:
	ScreenMain();

	virtual void Start();
	virtual void Update(float dt);
	virtual void ReceiveMessage(Message *message);

private:
	Actor *player;
    bool jumping;
    Actor *line;
    std::vector<Actor *> enemies;
};

