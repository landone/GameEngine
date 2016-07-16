#pragma once

#include "Entity.h"

/*
		SPOTLIGHT
Light that points in a direction spherically
*/

class Spotlight : public Entity {
public:
	Spotlight();
	~Spotlight();

	virtual glm::vec3 GetColor() { return color; }
	virtual double GetIntensity() { return intensity; }
	virtual double GetRange() { return range; }
	virtual double GetRadius() { return radius; }
	virtual glm::vec3 GetDirection() { return glm::vec3(rotTrans.GetModel() * glm::vec4(direction, 1)); }
	
	virtual void SetColor(glm::vec3 col); virtual void SetColor(double r, double g, double b) { SetColor(glm::vec3(r, g, b)); }
	virtual void SetDirection(glm::vec3 dir); virtual void SetDirection(double x, double y, double z) { SetDirection(glm::vec3(x, y, z)); }
	virtual void SetIntensity(double val);
	virtual void SetRange(double val);
	virtual void SetRadius(double val);

	virtual void Teleport(glm::vec3 pos); virtual void Teleport(double x, double y, double z) { Teleport(glm::vec3(x, y, z)); }
	virtual void Move(glm::vec3 amount); virtual void Move(double x, double y, double z) { Move(glm::vec3(x, y, z)); }

	virtual void Rotate(glm::vec3 amt);  void Rotate(double x, double y, double z) { Rotate(glm::vec3(x, y, z)); }
	virtual void SetRotation(glm::vec3 rot); void SetRotate(double x, double y, double z) { SetRotation(glm::vec3(x, y, z)); }

	virtual void Enable();
	virtual void Disable();
protected:
	double intensity = 1.0, range = 16.0, radius = 0.4;
	glm::vec3 color = glm::vec3(1, 1, 1), direction = glm::vec3(0, 0, -1);
	Transform rotTrans;
	virtual void SendData();
};