#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model(String name, String mesh, String texture) : AGameObject(name)
{
	this->mesh = std::make_unique<Mesh>(mesh);
	this->texture = std::make_shared<Texture>(texture);
	this->transform = std::make_unique<Transform>();
}

Model::~Model()
{
}

void Model::update(float deltaTime)
{
	theta += 0.001f;
}

void Model::draw(GLuint shaderProgram)
{
	this->transform.get()->rotate(0.0f, 0.016666f, 0.0f);
	glm::mat4 model = this->transform.get()->getTransformMatrix();

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
		0.1f,
		100.0f
	);

	glUseProgram(shaderProgram);

	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	GLuint tex0Loc = glGetUniformLocation(shaderProgram, "tex0");
	glBindTexture(GL_TEXTURE_2D, this->texture->getTexture());
	glUniform1i(tex0Loc, 0);

	this->mesh->draw();
}


