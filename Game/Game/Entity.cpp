#include "Entity.h"
#include "World.h"

Entity::Entity() {
	World::RegEnt(this);
}

Entity::~Entity() {

}

void Entity::Update() {
	if (parent != nullptr) {
		if (offsActive) {
			Teleport(offset.GetPos() + parent->GetPos());
			SetRotation(offset.GetRot() + parent->GetRot());
		}
		else {
			Teleport(parent->GetPos());
			SetRotation(parent->GetRot());
		}
		return;
	}
	if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0) {//Faster than finding length of vector
		Move(velocity);
	}
}

void Entity::SetParent(Entity& ent, bool offs) {
	if (&ent == this) {
		Console::Warning("Attempted to parent to self!");
		std::cout << classname << " to " << ent.GetClassname() << std::endl;
		return;
	}
	if (ent.GetParent() == this) {
		Console::Warning("Attempted to parent self to child!");
		std::cout << classname << " to " << ent.GetClassname() << std::endl;
	}
	offsActive = offs;
	if (offs) {
		offset.SetPos(position - ent.GetPos());
		offset.SetRot(rotation - ent.GetRot());
	}
	parent = &ent;
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