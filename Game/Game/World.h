#pragma once

#include "Entity.h"
#include "Wall.h"

/*
		WORLD
Handle collisions
*/

class World {
public:
	World() {}
	static World* World::Global();

	static void CalcCollisions();

	static void RegWall(Entity* wall);
	static void RemoveWall(Entity* wall);
	static void SetPlayer(Entity* player);
};