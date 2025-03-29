#include "Game.h"

#include <stdlib.h>
#include "config.h"

Game::Game()
{
    if (!glfwInit())
    {
        exit(-1);
    }

    this->gameWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scene Viewer", NULL, NULL);

    if (!gameWindow)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(gameWindow);
    gladLoadGL();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::run()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glfwSetTime(0.0f);

    while (!glfwWindowShouldClose(gameWindow))
    {
        this->processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->render();

        glfwSwapBuffers(gameWindow);

        glfwPollEvents();
    }

    glfwTerminate();
}

void Game::processInput()
{
}

void Game::update(float deltaTime)
{
}

void Game::render()
{
}
