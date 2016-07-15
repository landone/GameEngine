#pragma once
/*
		COMMON FUNCTIONS
Mostly mathematical operations to organize code
*/

#include <glm\glm.hpp>

class ComFunc {
public:
	static glm::vec3 ClosestPointOnLine(glm::vec3 line, glm::vec3 point);
	static double GetAngle(glm::vec2 origin, glm::vec2 targ);
	static glm::vec2 GetXZ(glm::vec3 temp);
	static glm::vec3 LinePlaneIntersect(glm::vec3 normal, glm::vec3 planePoint, glm::vec3 line, glm::vec3 origin);
	static bool IsPointOnFace(glm::vec3 point, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);//v1 must be in middle of the other two vertices
	static double GetRandomDouble(double min, double max);
	static int GetRandomInt(int min, int max);
};