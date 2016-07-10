#pragma once
/*
	COMMON FUNCTIONS
Mostly mathematical operations.
*/

#include <glm\glm.hpp>

class ComFunc {
public:
	static glm::vec3 ClosestPointOnLine(glm::vec3 line, glm::vec3 point);
	static double GetAngle(glm::vec2 origin, glm::vec2 targ);
	static glm::vec2 GetXZ(glm::vec3 temp);
};