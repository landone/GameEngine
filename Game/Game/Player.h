#pragma once

#include "Entity.h"
#include <SDL2\SDL.h>

class Player : public Entity {
public:
	Player();
protected:
	virtual void MouseMotion(double x, double y);
	virtual void KeyEvent(SDL_Keycode key, bool press);

	double sensitivity = 20;
};