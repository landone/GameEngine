#include "Player.h"
#include "Events.h"
#include "ComFunc.h"
#include "World.h"

static Player* global_player;
Player* Player::Global() {
	return global_player;
}

Player::Player() {
	Events::Listen(Evt_MouseMotion, this);
	Events::Listen(Evt_KeyEvent, this);
	camera->Init(glm::vec3(0, height, 0), 70.0f, 16.0 / 9.0, 0.01f, 1000.0f);
	World::SetPlayer(this);
	classname = "Player";
	global_player = this;
}

void Player::MouseMotion(double x, double y) {
	//camera->Rotate(x / sensitivity);
	//camera->Yaw(-y / sensitivity);
	camera->Rotate(-y / sensitivity, -x / sensitivity, 0);
	rotation = camera->GetRot();
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
		if (!buttons[4] && press && onGround) {
			velocity.y += (float)jumpBoost;
			Events::Send_OnPlayerJump();
		}
		buttons[4] = press;
		break;
	}
}

void Player::ApplyVelocity() {
	int forward = (int)buttons[0] - (int)buttons[2];
	int sides = (int)buttons[3] - (int)buttons[1];
	if (forward != 0 || sides != 0) {//Move based off facing direction & buttons
		double angle = ComFunc::GetAngle(glm::vec2(0, 0), ComFunc::GetXZ(camera->GetForward()));
		glm::vec3 result(cos(angle)*forward + cos(angle + PI/2)*sides, 0, sin(angle)*forward + sin(angle + PI/2)*sides);
		result = glm::normalize(result) * (float)walkAccel * (onGround ? 1.0f : 0.5f);
		velocity += result;
		glm::vec2 vel2 = ComFunc::GetXZ(velocity);
		float dot = glm::dot(ComFunc::GetXZ(result), glm::normalize(vel2));
		if (glm::length(vel2) > maxWalkSpeed && dot > 0.0f) {
			vel2 = glm::normalize(vel2);
			//velocity -= glm::normalize(velocity) * dot;
			velocity -= glm::vec3(vel2.x * dot, 0, vel2.y * dot);
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
	velocity.y -= gravity;
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