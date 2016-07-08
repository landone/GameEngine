#include "Player.h"
#include "display.h"

Player::Player() {
	Display* disp = Display::Global();
}

void Player::MouseMotion(double x, double y) {
	camera->Rotate(x / sensitivity);
	camera->Yaw(-y / sensitivity);
}

void Player::KeyEvent(SDL_Keycode key, bool press) {
	if (press) {
		switch (key) {
		case SDLK_w:
			camera->GetPos().x += 0.1f;
			break;
		case SDLK_s:
			camera->GetPos().x -= 0.1f;
			break;
		case SDLK_d:
			camera->GetPos().z += 0.1f;
			break;
		case SDLK_a:
			camera->GetPos().z -= 0.1f;
			break;
		case SDLK_SPACE:
			camera->GetPos().y += 0.1f;
			break;
		case SDLK_1:
			break;
		}
	}
}