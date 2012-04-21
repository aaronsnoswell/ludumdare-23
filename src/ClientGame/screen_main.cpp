
#include "stdafx.h"
#include "screen_main.h"

ScreenMain::ScreenMain() {
    // Create the ground
    line = new Actor();
    line->SetSize(100.0f, 2.0f);
    line->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    theWorld.Add(line);
    
    for(int i=0; i<10; i++) {
        Actor *tmp = new Actor();
        tmp->SetSize(2.0f);
        tmp->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
        tmp->SetPosition(2.0 * floor((rand() % 100 + 20) / 2.0f), 0);
        theWorld.Add(tmp);
        enemies.push_back(tmp);
    }

    // Create the player
    player = new Actor();
	player->SetSize(2.0f);
	player->SetColor(0.8824f, 0.0f, 0.4392f, 1.0f);
	theWorld.Add(player);
	
    // Subscribe to player messages
    theSwitchboard.SubscribeTo(this, "playerJump");
    theSwitchboard.SubscribeTo(this, "playerJumpUpDone");
    theSwitchboard.SubscribeTo(this, "playerJumpDownDone");

    // Load the background music
    AngelSampleHandle music = theSound.LoadSample("Resources/Sounds/xylophone.ogg", false);
    AngelSoundHandle soundPlaying = theSound.PlaySound(music); 
}

void ScreenMain::Start() {
    _objects.push_back(player);
    _objects.push_back(line);

    for(std::vector<Actor *>::iterator it = enemies.begin(); it != enemies.end(); it++) {
        _objects.push_back(*it);
    }
}

void ScreenMain::Update(float dt) {
    for(std::vector<Actor *>::iterator it = enemies.begin(); it != enemies.end(); it++) {
        (*it)->SetPosition((*it)->GetPosition() + *(new Vector2(-9.0f * dt, 0.0f)));

        if((*it)->GetPosition().X < -20) {
            (*it)->SetPosition(2.0 * floor((rand() % 100 + 20) / 2.0f), 0);
        }

        if(!jumping) {
            if(abs((*it)->GetPosition().X) < 2.0f) {
                theGame.ChangeScreen(MyGameManager::SCREEN_MAIN);
            }
        }
    }
}

void ScreenMain::ReceiveMessage(Message *message) {
    if(message->GetMessageName() == "PlayerJump") {
        if(jumping) return;
        jumping = true;
        player->ChangeSizeTo(5.0f, 0.5f, true, "PlayerJumpUpDone");
    } else if(message->GetMessageName() == "PlayerJumpUpDone") {
        player->ChangeSizeTo(2.0f, 0.5f, true, "PlayerJumpDownDone");
    } else if(message->GetMessageName() == "PlayerJumpDownDone") {
        jumping = false;
    }
    
}





