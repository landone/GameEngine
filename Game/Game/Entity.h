#pragma once

#include <time.h>
#include <glm/glm.hpp>
#include "camera.h"
#include "shader.h"
#include "transform.h"
#include "Events.h"

/*
			ENTITY
Parent class for most game objects
*/

#define TICK	10

class Entity : public Listener {
public:
	Entity();
	~Entity();

	virtual glm::vec3 GetPos() { return position; }
	virtual glm::vec3 GetRot() { return rotation; }
	virtual glm::vec3 GetVel() { return velocity; }
	virtual double GetScale() { return scale; }
	virtual Transform& GetTransform() { return transform; }
	virtual Entity* GetParent() { return parent; }
	virtual bool IsEnabled() { return enabled; }

	virtual void Draw() {}
	virtual void Update();

	virtual void Teleport(glm::vec3 pos); virtual void Teleport(double x, double y, double z);
	virtual void Move(glm::vec3 amount); virtual void Move(double x, double y, double z);
	virtual void SetRotation(glm::vec3 rot); virtual void SetRotation(double x, double y, double z);
	virtual void Rotate(glm::vec3 amount); virtual void Rotate(double x, double y, double z);
	virtual void SetScale(double factor); virtual void Scale(double factor);
	virtual void SetVel(glm::vec3 amt) { velocity = amt; } virtual void SetVel(double x, double y, double z) { SetVel(glm::vec3(x,y,z)); }

	virtual std::string GetClassname() { return classname; }

	virtual void SetParent(Entity& ent, bool offset = false);
	virtual void RemoveParent() { parent = nullptr; offsActive = false; }

	virtual void Enable();
	virtual void Disable();
protected:
	glm::vec3 position, rotation, velocity;
	double scale = 1.0;
	bool enabled = true, offsActive = false;
	Shader* shader = Shader::Global();
	Camera* camera = Camera::Global();
	Transform transform;
	std::string classname = "Entity";
	Transform offset;
	Entity* parent = nullptr;
};