#include "Spotlight.h"

Spotlight::Spotlight() {
	classname = "Spotlight";
	rotTrans.SetScale(glm::vec3(1, 1, 1));
	rotTrans.SetPos(glm::vec3(0, 0, 0));
	rotTrans.SetRot(glm::vec3(0, 0, 0));
	SendData();
}

Spotlight::~Spotlight() {
	Disable();
}

void Spotlight::SetColor(glm::vec3 amount) {
	color = amount;
	SendData();
}

void Spotlight::SetDirection(glm::vec3 amount) {
	direction = amount;
	rotTrans.SetRot(glm::vec3(0, 0, 0));
	SendData();
}

void Spotlight::SetIntensity(double amount) {
	intensity = amount;
	SendData();
}

void Spotlight::SetRange(double amount) {
	range = amount;
	SendData();
}

void Spotlight::SetRadius(double amount) {
	radius = amount;
	SendData();
}

void Spotlight::Teleport(glm::vec3 pos) {
	position = pos;
	transform.SetPos(position);
	SendData();
}

void Spotlight::Move(glm::vec3 amount) {
	position += amount;
	transform.SetPos(position);
	SendData();
}

void Spotlight::Rotate(glm::vec3 amt) {
	rotTrans.GetRot() += amt;
	SendData();
}

void Spotlight::SetRotation(glm::vec3 amt) {
	rotTrans.SetRot(amt);
	SendData();
}

void Spotlight::Enable() {
	if (enabled) { return; }
	enabled = true;
	SendData();
}

void Spotlight::Disable() {
	if (!enabled) { return; }
	enabled = false;
	Shader::RemoveSpotlight((char*)this);
}

void Spotlight::SendData() {
	if (!enabled) { return; }
	Shader::SetSpotlight((char*)this, color, glm::vec3(rotTrans.GetModel() * glm::vec4(direction, 1)), intensity, range, radius);
}