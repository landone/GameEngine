#include "Entity.h"
#include "World.h"

Entity::Entity() {
	World::RegEnt(this);
}

Entity::~Entity() {

}

void Entity::Update() {
	if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0) {//Faster than finding length of vector
		Move(velocity);
	}
}

void Entity::Teleport(glm::vec3 pos) {
	position = pos;
	transform.SetPos(position);
}

void Entity::Teleport(double x, double y, double z) {
	Teleport(glm::vec3(x, y, z));
}

void Entity::Move(glm::vec3 amount) {
	position += amount;
	transform.SetPos(position);
}

void Entity::Move(double x, double y, double z) {
	Move(glm::vec3(x,y,z));
}

void Entity::SetRotation(glm::vec3 rot) {
	rotation = rot;
	transform.SetRot(rot);
}

void Entity::SetRotation(double x, double y, double z) {
	SetRotation(glm::vec3(x, y, z));
}

void Entity::Rotate(glm::vec3 amount) {
	rotation += amount;
	transform.SetRot(rotation);
}

void Entity::Rotate(double x, double y, double z) {
	Rotate(glm::vec3(x, y, z));
}

void Entity::SetScale(double amount) {
	scale = amount;
	transform.SetScale(glm::vec3(scale, scale, scale));
}

void Entity::Scale(double factor) {
	scale *= factor;
	SetScale(scale);
}

void Entity::Enable() {
	enabled = true;
}

void Entity::Disable() {
	enabled = false;
}