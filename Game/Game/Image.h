#pragma once

#include <string>
#include "GL/glew.h"
#include "shader.h"
#include "transform.h"
#include "texture.h"
#include "mesh.h"

/*
			IMAGE
Image that can be drawn 2D or 3D
*/

class Image {
public:
	Image();
	void Init2D(double xco, double yco, double width, double height, Texture& texture);
	void Init3D(double xco, double yco, double width, double height, Texture& texture);
	void Draw(); void Draw3D();
	void Teleport(double xco, double yco) {
		xco = (xco / 800.0) - 1;
		yco = (yco / 450.0) - 1;
		m_transform.GetPos().x = (float)xco;
		m_transform.GetPos().y = (float)yco;
	}
	void Teleport(double xco, double yco, double zco) {
		m_transform.GetPos().x = (float)xco;
		m_transform.GetPos().y = (float)yco;
		m_transform.GetPos().z = (float)zco;
	}
	void Rotate(double xco, double yco, double zco) {//IN RADIANS
		m_transform.GetRot().x = (float)xco;
		m_transform.GetRot().y = (float)yco;
		m_transform.GetRot().z = (float)zco;
		if (m_transform.GetRot().x > 2.0f * PI) { m_transform.GetRot().x -= 2.0f * PI; }
		if (m_transform.GetRot().y > 2.0f * PI) { m_transform.GetRot().y -= 2.0f * PI; }
		if (m_transform.GetRot().z > 2.0f * PI) { m_transform.GetRot().z -= 2.0f * PI; }
		if (m_transform.GetRot().x < -2.0f * PI) { m_transform.GetRot().x += 2.0f * PI; }
		if (m_transform.GetRot().y < -2.0f * PI) { m_transform.GetRot().y += 2.0f * PI; }
		if (m_transform.GetRot().z < -2.0f * PI) { m_transform.GetRot().z += 2.0f * PI; }
	}
	double width, height;
protected:
private:
	std::vector<Vertex> v;
	std::vector<GLuint> in;
	Mesh m_mesh;
	Camera* camera;
	Shader* m_shader;
	Transform m_transform;
	Texture* m_texture;
	bool m_transparent;
};