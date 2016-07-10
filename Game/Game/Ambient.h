#pragma once

#include "Entity.h"

/*
		AMBIENT
Basic spherical light
*/

class Ambient : public Entity {
public:
	Ambient();
	~Ambient();

	virtual glm::vec3 GetColor() { return color; }
	virtual double GetIntensity() { return intensity; }
	virtual double GetRange() { return range; }

	virtual void SetColor(glm::vec3 amount); virtual void SetColor(double r, double g, double b);
	virtual void SetIntensity(double amount);
	virtual void SetRange(double amount);
	virtual void Teleport(glm::vec3 pos); virtual void Teleport(double x, double y, double z) { Teleport(glm::vec3(x, y, z)); }
	virtual void Move(glm::vec3 amount); virtual void Move(double x, double y, double z) { Move(glm::vec3(x, y, z)); }

	virtual void Enable();
	virtual void Disable();
protected:
	glm::vec3 color = glm::vec3(1,1,1);
	double intensity = 1, range = 10;
	virtual void SendData();
};