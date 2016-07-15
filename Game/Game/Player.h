#pragma once

#include "Entity.h"

/*
					PLAYER
Client-side player class for all player interactions
*/

class Player : public Entity {
public:
	Player();
	static Player* Global();
	virtual void ApplyVelocity();

	virtual void Move(glm::vec3 amount); virtual void Move(double x, double y, double z) { Move(glm::vec3(x, y, z)); }
	virtual void Teleport(glm::vec3 pos); virtual void Teleport(double x, double y, double z) { Teleport(glm::vec3(x, y, z)); }
	virtual glm::vec3 GetEyePos() { return glm::vec3(position.x, position.y + height, position.z); }
	virtual double GetHeight() { return height; }
	virtual glm::vec3 GetEyeDirection() { return camera->GetForward(); }
protected:
	virtual void MouseMotion(double x, double y);
	virtual void KeyEvent(SDL_Keycode key, bool press);

	bool buttons[5];
	bool onGround = true;
	double sensitivity = 20;
	double maxWalkSpeed = 0.12, walkAccel = 0.03, friction = 0.015;
	double height = 0.7, jumpBoost = 0.12;
};