#include "camera.h"

#define RIGHT_ANGLE		1.553343034274953323f

static Camera global_camera;
Camera* Camera::Global() {
	return &global_camera;
}

void Camera::ChangeAspect(double W, double H){
	m_aspect = (W / H);
	m_perspective = glm::perspective(m_fov, m_aspect , m_znear, m_zfar);
}

void Camera::Rotate(glm::vec3 amt) {
	glm::vec3& ref = transform.GetRot();
	ref += amt;
	if (ref.x > RIGHT_ANGLE) { ref.x = RIGHT_ANGLE; }
	if (ref.x < -RIGHT_ANGLE) { ref.x = -RIGHT_ANGLE; }
	m_forward = glm::vec3(transform.GetModel() * glm::vec4(0, 0, -1, 1));
}

void Camera::Move(double f, double r){//Forward, Right relative to viewing angle
	double rad;
	if (f != 0){
		rad = (m_rot * PI / 180);
		m_position.x += float(cos(rad) * f);
		m_position.z += float(sin(rad) * f);
	}
	if (r != 0){
		rad = ((m_rot + 90) * PI / 180);
		m_position.x += float(cos(rad) * r);
		m_position.z += float(sin(rad) * r);
	}
}

void Camera::Teleport(double x, double y, double z){
	m_position.x = (float)x;
	m_position.y = (float)y;
	m_position.z = (float)z;
}