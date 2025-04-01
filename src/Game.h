#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TestModel.h"
#include "Shader.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	void processInput();
	void update(float deltaTime);
	void render();

	void initializeWindow();
private:
	GLFWwindow* gameWindow;
	TestModel* model = nullptr;
	Shader* shader = nullptr;
};

