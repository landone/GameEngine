#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "transform.h"

/*
				CAMERA
Major difference between 2D and 3D drawing
*/

#define PI		3.1415926535f

class Camera{
public:
	Camera(const glm::vec3& pos, double fov, double aspect, double zNear, double zFar){
		Init(pos, fov, aspect, zNear, zFar);
	}
	Camera(){}
	void Init(const glm::vec3& pos, double fov, double aspect, double zNear, double zFar){
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
		m_position = pos;
		m_forward = glm::vec3(0, 0, -1);
		m_up = glm::vec3(0, 1, 0);
		m_rot = 270;
		m_yaw = 0;
		m_fov = fov; m_aspect = aspect; m_znear = zNear; m_zfar = zFar;
		transform.SetPos(glm::vec3(0,0,0));
		transform.SetScale(glm::vec3(1, 1, 1));
	}

	void ChangeAspect(double W, double H);
	static Camera* Global();

	void Move(double forward, double right = 0);
	void Teleport(double x, double y, double z);

	inline glm::vec3& GetPos(){ return m_position; }
	inline glm::vec3& GetForward(){ return m_forward; }
	inline glm::vec3& GetUp(){ return m_up; }
	inline glm::vec3& GetRot() { return transform.GetRot(); }

	void Rotate(glm::vec3 amt); void Rotate(double x, double y, double z) { Rotate(glm::vec3(x, y, z)); }

	inline glm::mat4 GetViewProjection() const {
		return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
	}
protected:
private:
	glm::mat4 m_perspective;
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	Transform transform;
	double m_rot, m_yaw;//Current rotation
	double m_fov, m_aspect, m_znear, m_zfar;
};