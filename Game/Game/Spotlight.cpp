#include "Spotlight.h"

Spotlight::Spotlight() {
	classname = "Spotlight";
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
	Shader::SetSpotlight((char*)this, color, direction, intensity, range, radius);
}