#include "Image.h"

Image::Image() {
	v.resize(4);
	in.resize(6);
	m_shader = Shader::Global();
	camera = Camera::Global();
}

void Image::Init2D(double xco, double yco, double w, double h, Texture& tex) {
	xco = (xco / 800.0) - 1;//Change from 1600x900 to -1:1
	yco = (yco / 450.0) - 1;
	m_transform.GetPos().x = (float)xco;
	m_transform.GetPos().y = (float)yco;
	width = w;
	height = h;
	w /= 800.0;
	h /= 450.0;

	v[0].position = glm::vec3(0, h, 0); v[0].texCoord = glm::vec2(0, 0); v[0].normal = glm::vec3();
	v[1].position = glm::vec3(0, 0, 0); v[1].texCoord = glm::vec2(0, 1.0); v[1].normal = glm::vec3();
	v[2].position = glm::vec3(w, 0, 0); v[2].texCoord = glm::vec2(1.0, 1.0); v[2].normal = glm::vec3();
	v[3].position = glm::vec3(w, h, 0); v[3].texCoord = glm::vec2(1.0, 0); v[3].normal = glm::vec3();
	in[0] = 0; in[1] = 1; in[2] = 2; in[3] = 2; in[4] = 3; in[5] = 0;

	m_mesh.Init(v, in);
	m_texture = &tex;
	if (m_texture->pathway.find(".png") != std::string::npos) { m_transparent = true; }
	else { m_transparent = false; }
}

void Image::Init3D(double xco, double yco, double w, double h, Texture& tex) {
	width = w;
	height = h;
	v[0].position = glm::vec3(0, h, 0); v[0].texCoord = glm::vec2(0, 0); v[0].normal = glm::vec3();
	v[1].position = glm::vec3(0, 0, 0); v[1].texCoord = glm::vec2(0, 1.0); v[1].normal = glm::vec3();
	v[2].position = glm::vec3(w, 0, 0); v[2].texCoord = glm::vec2(1.0, 1.0); v[2].normal = glm::vec3();
	v[3].position = glm::vec3(w, h, 0); v[3].texCoord = glm::vec2(1.0, 0); v[3].normal = glm::vec3();
	in[0] = 0; in[1] = 1; in[2] = 2; in[3] = 2; in[4] = 3; in[5] = 0;
	m_transform.GetPos().x = (float)xco;
	m_transform.GetPos().y = (float)yco;
	m_transform.GetPos().z = 0;

	m_mesh.Init(v, in);
	m_texture = &tex;
	if (m_texture->pathway.find(".png") != std::string::npos) { m_transparent = true; }
	else { m_transparent = false; }
}

void Image::Draw() {//Without camera
	glDisable(GL_DEPTH_TEST);
	if (m_transparent) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	int last = m_shader->effect;
	m_shader->SetEffect(1);//Fullbright
	m_shader->Bind();
	m_texture->Bind();
	m_shader->Update(m_transform);
	m_mesh.Draw();
	m_shader->SetEffect(last);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}

void Image::Draw3D() {
	if (m_transparent) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	m_shader->Bind();
	m_texture->Bind();
	m_shader->Update(m_transform, *camera);
	m_mesh.Draw();
}