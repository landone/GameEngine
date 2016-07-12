#pragma once
#include "World.h"
#include "Wall.h"
#include "Player.h"
#include "ComFunc.h"
#include <algorithm>

static World global_world;
World* World::Global() {
	return &global_world;
}

static Player* player;

//static std::vector<Wall*> walls;
static int wallCount = 0;
static Wall* walls[64];


void World::RegWall(Entity* wall) {
	Wall* ref = (Wall*)wall;
	for (Wall* temp : walls) {//Prevent duplicate wall pointers
		if (temp == ref) {
			return;
		}
	}
	walls[wallCount] = (Wall*)wall;
	wallCount++;
	//walls.push_back(ref);
}

void World::RemoveWall(Entity* wall) {
	Wall* ref = (Wall*)wall;
	//walls.erase(std::remove(walls.begin(), walls.end(), ref), walls.end());//Removes ref from vector
}

void World::SetPlayer(Entity* ply) {
	player = (Player*)ply;
}

void World::CalcCollisions() {//[TODO: ALIGN THIS WITH TICKS]
	glm::vec3 plpos = player->GetPos(), pleyes = player->GetEyePos(), plvel = player->GetVel();
	double mag = glm::length(plvel);
	glm::vec3 temp;
	for (Wall* wall : walls) {
		if (wall == nullptr || wall == 0) { continue; }
		temp = wall->GetPos();
		if (glm::distance(temp, plpos) >= sqrt(pow(wall->GetWidth(), 2) + pow(wall->GetHeight(), 2)) + player->GetHeight()) {//If cannot reach, skip this wall [TODO: MAKE THIS A VARIABLE TO LIMIT PROCESSING]
			continue;
		}
		glm::vec3 normal = wall->GetNormal();
		glm::vec3 point = ComFunc::LinePlaneIntersect(normal, temp, plvel, plpos);//FIRST CHECK FOR FEET COLLISION
		if (point == plpos) { continue; }//Skip if perpendicular
		if (mag >= glm::distance(point, plpos) - 0.33 && glm::dot(plvel, glm::normalize(point - plpos)) > 0 && ComFunc::IsPointOnFace(point, temp, wall->GetVert(0), wall->GetVert(2))) {
			//Player is about to run into wall [TODO: ADD PLAYER WIDTHRADIUS TO EQUATION]
			plvel -= normal * glm::dot(plvel, normal);
		}
		point = ComFunc::LinePlaneIntersect(normal, temp, plvel, pleyes);//SECOND CHECK FOR EYE COLLISION
		if (point == pleyes) { continue; }
		if (mag >= glm::distance(point, pleyes) - 0.33 && glm::dot(plvel, glm::normalize(point - pleyes)) > 0 && ComFunc::IsPointOnFace(point, temp, wall->GetVert(0), wall->GetVert(2))) {
			plvel -= normal * glm::dot(plvel, normal);
		}
	}
	player->SetVel(plvel);
}