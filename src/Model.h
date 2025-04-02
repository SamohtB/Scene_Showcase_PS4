#pragma once
#include "config.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "AGameObject.h"

class Mesh;
class Transform;
class Texture;

class Model : public AGameObject
{
public:
	using String = std::string;

	Model(String name, String mesh, String texture);
	~Model();

	void draw(GLuint shaderProgram);

	Transform* getTransform();
	Mesh* getMesh();
	Texture* getTexture();

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::unique_ptr<Transform> transform;
	float theta = 0.0f;

	// Inherited via AGameObject
	void update(float deltaTime) override;
};

