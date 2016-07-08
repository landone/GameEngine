#include "camera.h"

static Camera global_camera;
Camera* Camera::Global() {
	return &global_camera;
}

void Camera::ChangeAspect(double W, double H){
	m_aspect = (W / H);
	m_perspective = glm::perspective(m_fov, m_aspect , m_znear, m_zfar);
}

void Camera::Rotate(double degrees){
	m_rot += degrees;
	if (m_rot >= 360){
		m_rot -= 360;
	}
	else if (m_rot < 0){
		m_rot += 360;
	}

	m_forward.x = (float)cos(m_rot * PI / 180);
	m_forward.z = (float)sin(m_rot * PI / 180);
}

void Camera::Yaw(double degrees){//Bounded at 90 degrees
	m_yaw += degrees;
	if (m_yaw > 90){
		m_yaw = 90;
	}
	else if (m_yaw < -90){
		m_yaw = -90;
	}
	m_forward.y = (float)(m_yaw / 45);
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