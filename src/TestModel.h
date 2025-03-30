#pragma once
#include <string>
#include <memory>

#include <glad/glad.h>

class Mesh;
class Shader;
class Texture;

class TestModel
{
public:
	using String = std::string;

	TestModel(String mesh, String texture);
	~TestModel();

	void draw(GLuint shaderProgram);

private:
	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	float theta = 0.0f;
};

