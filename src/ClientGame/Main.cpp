
#include "stdafx.h"

#include "game_manager.h"

int main(int argc, char* argv[]) {
	// get things going
	//  optional parameters:
	//		int windowWidth			default: 1024
	//		int windowHeight		default: 768
	//		std::string windowName	default: "Angel Engine"
	//		bool antiAliasing		default: false
	//		bool fullScreen			default: false
	theWorld.Initialize(1024, 768, "Pix Blit Runner", true, false);
	
	//adds the default grid so you can more easily place Actors
	theWorld.Add(new GridActor(), -1);
	
    // Set the game manager to handle sounds and scenes etc
    theWorld.SetGameManager(&theGame);

	// do all your setup first, because this function won't return until you're exiting
	theWorld.StartGame();
	
	// any cleanup can go here
	theWorld.Destroy();
	
	return 0;
}
