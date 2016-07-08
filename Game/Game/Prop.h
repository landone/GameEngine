#pragma once

#include "Entity.h"
#include "texture.h"
#include "mesh.h"

class Prop : public Entity {
public:
	virtual void Draw();
	virtual void SetMesh(const std::string& path); virtual void SetMesh(Mesh& mesh);
	virtual Mesh& GetMesh() { return mesh; }
	virtual void SetTex(const std::string& path); virtual void SetTex(Texture& tex);
	virtual Texture& GetTex() { return texture; }
protected:
	Texture texture;
	Mesh mesh;
};