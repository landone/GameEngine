#include "ComFunc.h"
#include <time.h>

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

glm::vec3 ComFunc::LinePlaneIntersect(glm::vec3 normal, glm::vec3 planePoint, glm::vec3 line, glm::vec3 origin) {
	//origin + t * line = v
	//normal * v = d

	/*double d = glm::dot(planePoint, normal);
	double t = (d - glm::dot(origin, normal)) / glm::dot(line, normal);
	return origin + line * (float)t;*/

	float check = glm::dot(line, normal);
	if (check == 0) {//If never intersects, return origin
		return origin;
	}
	return origin + line * (glm::dot(planePoint - origin, normal) / check);
}

bool ComFunc::IsPointOnFace(glm::vec3 point, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	double rem1 = glm::dot(point, v2 - v1), rem2 = glm::dot(point, v3 - v1);
	if (glm::dot(v1, v2 - v1) <= rem1 && rem1 <= glm::dot(v2, v2 - v1) &&
		glm::dot(v1, v3 - v1) <= rem2 && rem2 <= glm::dot(v3, v3 - v1)) {
		return true;
	}
	return false;
}

double ComFunc::GetRandomDouble(double min, double max) {
	return sin((double)rand())*(max - min) + min;
}

int ComFunc::GetRandomInt(int min, int max) {
	return (int)round(GetRandomDouble(min, max));
}