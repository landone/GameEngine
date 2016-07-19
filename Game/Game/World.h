#pragma once

#include "Entity.h"
#include "Wall.h"

/*
					WORLD
Manages all entities (collisions, updates, tick rate)
*/

class World {
public:
	World() {}
	static World* World::Global();

	static void Update();
	static void CalcCollisions();

	static void RegEnt(Entity* entity);
	static void RemoveEnt(Entity* entity);

	static void RegWall(Entity* wall);
	static void RemoveWall(Entity* wall);

	static void SetPlayer(Entity* player);

	static glm::vec3 TraceRay(glm::vec3 origin, glm::vec3 direction, double maxDistance = 0.0);

	static void Test();//Arbitrary DEBUG function
};