#include "Prism.h"

Prism::Prism(double w, double h, double d) {
	classname = "Prism";
	walls[0].prismPart = true;
	walls[1].prismPart = true;
	walls[2].prismPart = true;
	walls[3].prismPart = true;
	walls[4].prismPart = true;
	walls[5].prismPart = true;
	walls[0].SetRotation(PI / 2, 0, 0);
	walls[1].SetRotation(0, -PI / 2, 0);
	walls[3].SetRotation(0, -PI / 2, 0);
	walls[5].SetRotation(PI / 2, 0, 0);
	SetDimensions(glm::vec3(w, h, d));
}

void Prism::SetDimensions(glm::vec3 amt) {
	dimensions = amt;
	walls[0].SetSize(amt.x, amt.z);
	walls[1].SetSize(amt.z, amt.y);
	walls[2].SetSize(amt.x, amt.y);
	walls[3].SetSize(amt.z, amt.y);
	walls[3].Teleport(position + glm::vec3(offset.GetModel() * glm::vec4(amt.x, 0.0f, 0.0f, 1.0f)));
	walls[4].SetSize(amt.x, amt.y);
	walls[4].Teleport(position + glm::vec3(offset.GetModel() * glm::vec4(0.0f, 0.0f, amt.z, 1.0f)));
	walls[5].SetSize(amt.x, amt.z);
	walls[5].Teleport(position + glm::vec3(offset.GetModel() * glm::vec4(0.0f, amt.y, 0.0f, 1.0f)));
}

void Prism::Draw() {
	if (!enabled) { return; }
	for (int i = 0; i < 6; i++) {
		walls[i].Draw();
	}
}

void Prism::Teleport(glm::vec3 pos) {
	walls[0].Teleport(pos);
	walls[1].Teleport(pos);
	walls[2].Teleport(pos);
	glm::vec3 dif = pos - position;
	walls[3].Move(dif);
	walls[4].Move(dif);
	walls[5].Move(dif);
	position = pos;
}

void Prism::Teleport(double x, double y, double z) {
	Teleport(glm::vec3(x, y, z));
}

void Prism::Move(glm::vec3 amount) {
	for (int i = 0; i < 6; i++) {
		walls[i].Move(amount);
	}
	position += amount;
}

void Prism::Move(double x, double y, double z) {
	Move(glm::vec3(x, y, z));
}

Wall& Prism::GetWall(int num) {
	if (num < 0 || num > 5) {
		Console::Error("Attempted to get wall from prism outside of 0-5 range!");
		return walls[0];
	}
	return walls[num];
}

void Prism::SetRotation(glm::vec3 rot) {
	glm::vec3 dif = rot - rotation;
	offset.SetRot(rot);
	for (int i = 0; i < 6; i++) {//Bug on walls 1&3 with x rotation (thinks x is z rotation)
			walls[i].Rotate(dif);
	}
	walls[3].Teleport(position + glm::vec3(offset.GetModel() * glm::vec4(dimensions.x, 0, 0, 1)));
	walls[4].Teleport(position + glm::vec3(offset.GetModel() * glm::vec4(0, 0, dimensions.z, 1)));
	walls[5].Teleport(position + glm::vec3(offset.GetModel() * glm::vec4(0, dimensions.y, 0, 1)));
	rotation = rot;
}

void Prism::SetRotation(double x, double y, double z) {
	SetRotation(glm::vec3(x, y, z));
}

void Prism::Rotate(glm::vec3 amount) {
	SetRotation(rotation + amount);
}

void Prism::Rotate(double x, double y, double z) {
	Rotate(glm::vec3(x, y, z));
}

void Prism::SetScale(double amount) {
	scale = amount;
	for (int i = 0; i < 6; i++) {
		walls[i].SetScale(amount);
	}
}

void Prism::Scale(double factor) {
	scale *= factor;
	SetScale(scale);
}

void Prism::SetTex(Texture& tex) {
	for (int i = 0; i < 6; i++) {
		walls[i].SetTex(tex);
	}
}

void Prism::SetTex(const std::string& fileName) {
	Texture tex; tex.Load(fileName);
	SetTex(tex);
}