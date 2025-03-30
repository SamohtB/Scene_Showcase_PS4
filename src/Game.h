#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TestModel.h"
#include "Shader.h"

class Game
{
public:
	Game();
	void initializeWindow();
	void run();

private:
	void processInput();
	void update(float deltaTime);
	void render();

private:
	GLFWwindow* gameWindow;
	TestModel* model = nullptr;
	Shader* shader = nullptr;
};

