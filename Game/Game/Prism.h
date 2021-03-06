#pragma once

#include "Wall.h"

/*
		PRISM
Groups of walls in a prism
Individual walls are selectable
X-AXIS ROTATION DOES NOT WORK CURRENTLY
*/

class Prism : public Entity {
public:
	Prism(double width = 1, double height = 1, double depth = 1);

	virtual void Draw();
	virtual void SetTex(const std::string& fileName); virtual void SetTex(Texture& tex);
	virtual void Teleport(glm::vec3 pos); virtual void Teleport(double x, double y, double z);
	virtual void Move(glm::vec3 amount); virtual void Move(double x, double y, double z);
	virtual void SetRotation(glm::vec3 rot); virtual void SetRotation(double x, double y, double z);
	virtual void Rotate(glm::vec3 amount); virtual void Rotate(double x, double y, double z);
	virtual void SetScale(double amount); virtual void Scale(double factor);

	virtual Wall& GetWall(int num);
	virtual glm::vec3 GetDimensions() { return dimensions; }
	virtual void SetDimensions(glm::vec3 amt);
	virtual void SetDimensions(double width, double height, double depth) { SetDimensions(glm::vec3(width, height, depth)); }
protected:
	Wall walls[6];
	Transform offset;
	glm::vec3 dimensions;//w,h,d
};