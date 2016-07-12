#include "Player.h"
#include "Events.h"
#include "ComFunc.h"
#include "World.h"

Player::Player() {
	Events::Listen(Evt_MouseMotion, this);
	Events::Listen(Evt_KeyEvent, this);
	camera->Init(glm::vec3(0, height, 0), 70.0f, 16.0 / 9.0, 0.01f, 1000.0f);
	World::SetPlayer(this);
}

void Player::MouseMotion(double x, double y) {
	camera->Rotate(x / sensitivity);
	camera->Yaw(-y / sensitivity);
}

void Player::KeyEvent(SDL_Keycode key, bool press) {
	double temp = 0;
	switch (key) {
	case SDLK_w:
		buttons[0] = press;
		break;
	case SDLK_a:
		buttons[1] = press;
		break;
	case SDLK_s:
		buttons[2] = press;
		break;
	case SDLK_d:
		buttons[3] = press;
		break;
	case SDLK_SPACE:
		break;
	}
}

void Player::ApplyVelocity() {
	if (enabled && clock() > ticks + TICK) {
		int forward = (int)buttons[0] - (int)buttons[2];
		int sides = (int)buttons[3] - (int)buttons[1];
		if (forward != 0 || sides != 0) {//Move based off facing direction & buttons
			double angle = ComFunc::GetAngle(glm::vec2(0, 0), ComFunc::GetXZ(camera->GetForward()));
			glm::vec3 result(cos(angle)*forward + cos(angle + PI/2)*sides, 0, sin(angle)*forward + sin(angle + PI/2)*sides);
			result = glm::normalize(result) * (float)walkAccel;
			velocity += result;
			double dot = glm::dot(result, glm::normalize(velocity));
			if (glm::length(velocity) > maxWalkSpeed && dot > 0) {
				velocity -= glm::normalize(velocity) * (float)dot;
			}
		}
		if (onGround && (velocity.x != 0 || velocity.z != 0)) {//Apply friction
			glm::vec3 result(velocity.x, 0, velocity.z);
			glm::vec3 remember = velocity;
			result = glm::normalize(result) * (float)friction;
			velocity -= result;
			if (remember.x > 0 && velocity.x < 0) { velocity.x = 0; }
			if (remember.x < 0 && velocity.x > 0) { velocity.x = 0; }
			if (remember.z > 0 && velocity.z < 0) { velocity.z = 0; }
			if (remember.z < 0 && velocity.z > 0) { velocity.z = 0; }
		}
	}
}

void Player::Update() {
	if (enabled && clock() > ticks + TICK) {
		if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0) {//Faster than finding length of vector
			Move(velocity);
		}
		ticks = clock();
	}
}

void Player::Move(glm::vec3 amount) {
	position += amount;
	transform.SetPos(position);
	camera->GetPos() = position;
	camera->GetPos().y += (float)height;
}

void Player::Teleport(glm::vec3 pos) {
	position = pos;
	transform.SetPos(position);
	camera->GetPos() = position;
	camera->GetPos().y += (float)height;
}