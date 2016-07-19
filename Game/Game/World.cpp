#pragma once
#include "World.h"
#include "display.h"
#include "Wall.h"
#include "Player.h"
#include "Spotlight.h"
#include "ComFunc.h"
#include "Sound.h"
#include <vector>
#include <algorithm>

static World global_world;
World* World::Global() {
	return &global_world;
}

static Player* player = nullptr; 
static Display* display = Display::Global();
static clock_t ticks = 0;

static std::vector<Wall*> walls;
static std::vector<Entity*> entities;

void World::Update() {
	if (clock() >= ticks + TICK) {//10ms
		display->Clear(0.5, 0.5, 0.5, 1);
		if (player != nullptr) {
			player->ApplyVelocity();
		}
		Events::Send_OnTick();
		CalcCollisions();
		for (Entity* ent : entities) {
			if (!ent->IsEnabled()) { continue; }//Skip disabled
			if (ent->GetClassname() == "Wall" && ((Wall*)ent)->prismPart) { continue; }//Skip walls controlled by a prism
			ent->Update();
			ent->Draw();
		}
		display->Update();
		ticks = clock();
	}
}

void World::Test() {//DEBUG FUNCTION
}

void World::RegEnt(Entity* ent) {
	for (Entity* test : entities) {//Prevent duplicates
		if (test == ent) {
			return;
		}
	}
	entities.push_back(ent);
}

void World::RemoveEnt(Entity* ent) {
	entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
}


void World::RegWall(Entity* wall) {
	Wall* ref = (Wall*)wall;
	for (Wall* temp : walls) {//Prevent duplicate wall pointers
		if (temp == ref) {
			return;
		}
	}
	walls.push_back(ref);
}

void World::RemoveWall(Entity* wall) {
	Wall* ref = (Wall*)wall;
	walls.erase(std::remove(walls.begin(), walls.end(), ref), walls.end());//Removes ref from vector
}

void World::SetPlayer(Entity* ply) {
	player = (Player*)ply;
}

struct WallPair {
	Wall* ptr;
	float dist;
	glm::vec3 point;
};
static std::vector<WallPair> orgnzr;

void World::CalcCollisions() {//[TODO: CHECK WALLS FROM CLOSEST TO FURTHEST]
	if (player != nullptr) {
		orgnzr.clear();
		glm::vec3 plpos = player->GetPos(), pleyes = player->GetEyePos(), plvel = player->GetVel();
		float mag = glm::length(plvel), wrad = 0.33f;
		glm::vec3 temp, normal, point, amt;
		WallPair tempair;
		bool onground = false;
		for (Wall* wall : walls) {
			if (wall == 0 || wall == nullptr) { continue; }
			tempair.ptr = wall;
			temp = wall->GetPos();
			glm::vec3 normal = wall->GetNormal();
			glm::vec3 point = ComFunc::LinePlaneIntersect(normal, temp, plvel, plpos);
			if (point != plpos && glm::dot(plvel, glm::normalize(point - plpos)) > 0) {
				tempair.point = point;
			}else{
				continue;
			}
			tempair.dist = glm::distance(plpos, point);
			orgnzr.push_back(tempair);
		}
		int size = orgnzr.size();//Create a constant since size may change
		float tempDist;
		int id;
		for (int i = 0; i < size; i++) {
			tempDist = 0;
			id = 0;
			for (unsigned int u = 0; u < orgnzr.size(); u++) {//Find closest
				if (tempDist == 0 || orgnzr[u].dist < tempDist) {
					id = u;
					tempDist = orgnzr[u].dist;
				}
			}
			normal = orgnzr[id].ptr->GetNormal();
			point = orgnzr[id].point;
			temp = orgnzr[id].ptr->GetPos();
			if (mag >= glm::distance(point, plpos) - wrad && ComFunc::IsPointOnFace(point, temp, orgnzr[id].ptr->GetVert(0), orgnzr[id].ptr->GetVert(2))) {
				//Player is about to run into wall
				amt = normal * glm::dot(plvel, normal);
				if (abs(amt.y) > player->GetGravity() / 2) {//If pushing more than half gravity
					player->SetGroundEnt(orgnzr[id].ptr);
					onground = true;
				}
				plvel -= amt;
			}
			point = ComFunc::LinePlaneIntersect(normal, temp, plvel, pleyes);//SECOND CHECK FOR EYE COLLISION
			if (point != pleyes) {
				if (mag >= glm::distance(point, pleyes) - wrad && glm::dot(plvel, glm::normalize(point - pleyes)) > 0 && ComFunc::IsPointOnFace(point, temp, orgnzr[id].ptr->GetVert(0), orgnzr[id].ptr->GetVert(2))) {
					plvel -= normal * glm::dot(plvel, normal);
				}
			}
			orgnzr.erase(orgnzr.begin() + id);//Remove from array
		}
		if (!onground) { player->SetGroundEnt(nullptr); }
		player->SetVel(plvel);
	}
}

glm::vec3 World::TraceRay(glm::vec3 origin, glm::vec3 direction, double maxDistance) {
	glm::vec3 temp, result = origin;
	float distance = 0, tempDist;
	for (Wall* wall : walls) {
		if (wall == nullptr || wall == 0) { continue; }
		temp = ComFunc::LinePlaneIntersect(wall->GetNormal(), wall->GetPos(), direction, origin);
		if (temp == origin) { continue; }//Perpendicular
		tempDist = glm::distance(origin, temp);
		if ((maxDistance == 0 || tempDist <= maxDistance) && (distance == 0 || tempDist < distance) && glm::dot(direction, glm::normalize(temp - origin)) > 0) {
			distance = tempDist;
			result = temp;
		}
	}
	return result;
}