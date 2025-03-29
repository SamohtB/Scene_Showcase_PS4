#pragma once
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class AMesh
{
public:
	AMesh(std::string strMesh);
	~AMesh();

	GLuint GetVAO();
	GLuint GetVBO();
	std::vector<GLfloat> GetVertexData();

	void LoadMesh(std::string strMesh);

protected:
	GLuint VAO, VBO;
	std::vector<GLfloat> fullVertexData;
};

