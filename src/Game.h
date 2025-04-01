#pragma once

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
	Shader* shader = nullptr;
};

