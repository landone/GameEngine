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
	for (Entity* ent : entities) {
		if (ent->GetClassname() == "Sound") {
			Sound* snd = (Sound*)ent;
			snd->Play();
		}
	}
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

void World::CalcCollisions() {//[TODO: ALIGN THIS WITH TICKS & FIX FLOOR SINKING PROBLEM]
	if (player != nullptr) {
		glm::vec3 plpos = player->GetPos(), pleyes = player->GetEyePos(), plvel = player->GetVel();
		float mag = glm::length(plvel), wrad = 0.33f;
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
			if (mag >= glm::distance(point, plpos) - wrad && glm::dot(plvel, glm::normalize(point - plpos)) > 0 && ComFunc::IsPointOnFace(point, temp, wall->GetVert(0), wall->GetVert(2))) {
				//Player is about to run into wall
				plvel -= normal * glm::dot(plvel, normal);
			}
			point = ComFunc::LinePlaneIntersect(normal, temp, plvel, pleyes);//SECOND CHECK FOR EYE COLLISION
			if (point == pleyes) { continue; }
			if (mag >= glm::distance(point, pleyes) - wrad && glm::dot(plvel, glm::normalize(point - pleyes)) > 0 && ComFunc::IsPointOnFace(point, temp, wall->GetVert(0), wall->GetVert(2))) {
				plvel -= normal * glm::dot(plvel, normal);
			}
		}
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