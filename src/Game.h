#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Game
{
public:
	Game();
	void run();

private:
	void processInput();
	void update(float deltaTime);
	void render();

private:
	GLFWwindow* gameWindow;

};

