#include "Wall.h"

Wall::Wall(double w, double h, double x, double y) {
	width = w;
	height = h;
	texX = w / x;
	texY = h / y;

	std::vector<GLuint> in;
	v.resize(4);
	in.resize(12);
	v[0].position = glm::vec3(0, 1, 0); v[0].texCoord = glm::vec2(0.0, texY);
	v[1].position = glm::vec3(0, 0, 0); v[1].texCoord = glm::vec2(0.0, 0);
	v[2].position = glm::vec3(1, 0, 0); v[2].texCoord = glm::vec2(texX, 0);
	v[3].position = glm::vec3(1, 1, 0); v[3].texCoord = glm::vec2(texX, texY);
	in[0] = 0; in[1] = 1; in[2] = 2; in[3] = 2; in[4] = 3; in[5] = 0;
	in[6] = 2; in[7] = 1; in[8] = 0; in[9] = 0; in[10] = 3; in[11] = 2;
	normal = glm::vec3(0, 0, 1);
	v[0].normal = normal; v[1].normal = normal; v[2].normal = normal; v[3].normal = normal;

	mesh.Init(v, in);
}

void Wall::UpdateTexture() {
	v[0].texCoord = glm::vec2(0, texY);
	v[1].texCoord = glm::vec2(0, 0);
	v[2].texCoord = glm::vec2(texX, 0);
	v[3].texCoord = glm::vec2(texX, texY);
	mesh.UpdateVertices(v);
}

void Wall::SetTexSize(double x, double y) {
	texX = width / x;
	texY = height / y;
	UpdateTexture();
}

void Wall::SetScale(double amount) {
	scale = amount;
	transform.SetScale(glm::vec3(scale*width, scale*height, 1));
}

void Wall::SetSize(double w, double h) {
	texX *= w / width;
	texY *= h / height;
	UpdateTexture();
	width = w;
	height = h;
	transform.SetScale(glm::vec3(scale*width, scale*height, 1));
}

void Wall::SetWidth(double w) {
	texX *= w / width;
	UpdateTexture();
	width = w;
	transform.SetScale(glm::vec3(scale*width, scale*height, 1));
}

void Wall::SetHeight(double h) {
	texY *= h / height;
	UpdateTexture();
	height = h;
	transform.SetScale(glm::vec3(scale*width, scale*height, 1));
}

void Wall::Draw() {
	if (!enabled) { return; }
	shader->Bind();
	texture.Bind();
	//glDepthFunc(GL_ALWAYS);
	shader->Update(transform, *camera);
	//shader.SetFBright(1);
	mesh.Draw();
	//glDepthFunc(GL_LESS);
}

void Wall::SetTex(const std::string& path) {
	texture.Load(path);
}

void Wall::SetTex(Texture& tex) {
	texture = tex;
}