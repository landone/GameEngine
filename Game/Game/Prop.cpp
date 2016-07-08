#include "Prop.h"

void Prop::Draw() {
	if (!enabled) { return; }
	shader->Bind();
	texture.Bind();
	//glDepthFunc(GL_ALWAYS);
	shader->Update(transform, *camera);
	//shader.SetFBright(1);
	mesh.Draw();
	//glDepthFunc(GL_LESS);
}

void Prop::SetMesh(const std::string& path) {
	mesh.Load(path);
}

void Prop::SetMesh(Mesh& base) {
	mesh = base;
}

void Prop::SetTex(const std::string& path) {
	texture.Load(path);
}

void Prop::SetTex(Texture& tex) {
	texture = tex;
}
