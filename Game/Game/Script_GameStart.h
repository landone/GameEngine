#pragma once

#include "ScriptManager.h"

class GameStart : public Script {
public:
	GameStart();

	void Close();
	void MouseClick(double x, double y, int type);
private:
	Player player;
	Prism prism;
	Texture cube, blue, wood;
	Ambient amb;
	Spotlight spot;
	Sound light, error, radio;
};