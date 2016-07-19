#include "Script_GameStart.h"

GameStart::GameStart(){
	Events::Listen(Evt_MouseClick, this);

	cube.Load("./res/tex/cuboid.jpg");
	blue.Load("./res/tex/blueprint.jpg");
	wood.Load("./res/tex/woodoid.jpg");

	prism.SetDimensions(10, 10, 10);
	prism.Teleport(-5, -5, -5);
	prism.SetTex(wood);

	spot.SetColor(0, 1, 0);
	spot.SetDirection(player.GetEyeDirection());
	spot.Teleport(player.GetEyePos());
	spot.SetParent(player, true);

	light.Load("./res/sound/flashlight.wav"); light.SetPhysical(false);
	error.Load("./res/sound/error.wav"); error.SetPhysical(false);
	radio.Load("./res/sound/radio.wav"); radio.SetPhysical(false);
}

void GameStart::Close() {
	
}

void GameStart::MouseClick(double x, double y, int type) {
	if (type == 1) {
		light.Play();
		if (spot.IsEnabled()) {
			spot.Disable();
		}
		else {
			spot.Enable();
		}
	}
	else if (type == 2) {
		radio.Play();
	}
	else if (type == 3) {
		error.Play();
	}
}