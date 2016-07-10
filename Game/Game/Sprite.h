#pragma once

#include "Entity.h"
#include "Image.h"

/*
				SPRITE
3D image that rotates to always face the camera
*/

class Sprite : public Entity {
public:
	Sprite(Texture& tex, double w = 1, double h = 1) {
		Init(tex, w, h);
	}
	Sprite() {}

	virtual void Init(Texture& tex, double w = 1, double h = 1) {
		width = w; height = h;
		img.Init3D(0, 0, w, h, tex);
	}

	virtual void SetYield(bool set) { m_yield = set; }
	virtual void Draw() {
		glm::vec3 temp = cam->GetPos() - position;
		double result = atan(abs(temp.x) / abs(temp.z)), hyp = 0;
		if (temp.z < 0 && temp.x > 0) { result = PI - result; }
		if (temp.z < 0 && temp.x < 0) { result += PI; }
		if (temp.z > 0 && temp.x < 0) { result = 2 * PI - result; }
		if (m_yield) {
			hyp = sqrt(pow(temp.x, 2) + pow(cam->GetPos().z, 2));
			hyp = atan((cam->GetPos().y - position.y) / hyp);
		}
		img.Rotate(-hyp, result, 0);
		img.Teleport(position.x + sin(result)*width / 2, position.y, position.z + cos(result)*width / 2);
		img.Draw3D();
	}
protected:
	Image img;
	Camera* cam = Camera::Global();
	double width, height;
	bool m_yield = true;
};