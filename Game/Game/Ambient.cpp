#include "Ambient.h"

Ambient::Ambient() {
	SendData();
	classname = "Ambient";
}

Ambient::~Ambient() {
	Disable();
}

void Ambient::SetColor(glm::vec3 amount) {
	color = amount;
	SendData();
}

void Ambient::SetColor(double r, double g, double b) {
	SetColor(glm::vec3(r, g, b));
}

void Ambient::SetIntensity(double amount) {
	intensity = amount;
	SendData();
}

void Ambient::SetRange(double amount) {
	range = amount;
	SendData();
}

void Ambient::Teleport(glm::vec3 pos) {
	position = pos;
	transform.SetPos(position);
	SendData();
}

void Ambient::Move(glm::vec3 amount) {
	position += amount;
	transform.SetPos(position);
	SendData();
}

void Ambient::Enable() {
	if (enabled) { return; }
	enabled = true;
	SendData();
}

void Ambient::Disable() {
	if (!enabled) { return; }
	enabled = false;
	Shader::RemoveAmbient((char*)this);
}

void Ambient::SendData() {
	if (!enabled) { return; }
	Shader::SetAmbient((char*)this, color, intensity, range);
}