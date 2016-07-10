#include "ComFunc.h"

#define PI		3.1415926535f

glm::vec3 ComFunc::ClosestPointOnLine(glm::vec3 line, glm::vec3 point) {
	float t = glm::dot(point, line) / pow(glm::length(line), 2);
	if (t < 0) { return glm::vec3(0,0,0); }
	return line * t;
}

double ComFunc::GetAngle(glm::vec2 origin, glm::vec2 targ) {
	targ -= origin;
	if (targ.x == 0) {
		if (targ.y == 0) {
			return 0;
		}else if (targ.y > 0) {
			return PI / 2;
		}
		else {
			return 3 * PI / 2;
		}
	}
	double temp = atan(abs(targ.y / targ.x));
	if (targ.x < 0 && targ.y > 0) {
		temp = PI - temp;
	}
	else if (targ.x < 0 && targ.y < 0) {
		temp += PI;
	}
	else if (targ.x > 0 && targ.y < 0) {
		temp = 2 * PI - temp;
	}
	return temp;
}

glm::vec2 ComFunc::GetXZ(glm::vec3 temp) {
	return glm::vec2(temp.x, temp.z);
}