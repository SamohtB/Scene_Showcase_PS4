#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"

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
	Mesh* mesh = nullptr;

};

