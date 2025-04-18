#include "Game.h"

#include <stdlib.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "config.h"
#include "Debug.h"

#include "GameObjectManager.h"
#include "EventBroadcaster.h"
#include "UIManager.h"

#include "Model.h"
#include "LevelDataClient.h"

Game::Game()
{
    initializeWindow();
    this->shader = new Shader("Shaders/Shader.vert", "Shaders/Shader.frag");

    EventBroadcaster::initialize();
    Debug::initialize();
    UIManager::initialize(this->gameWindow);
    GameObjectManager::initialize();
}

Game::~Game()
{
    GameObjectManager::destroy();
    UIManager::destroy();
    Debug::destroy();
    EventBroadcaster::destroy();
}

void Game::initializeWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (!glfwInit())
    {
        exit(-1);
    }

    this->gameWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scene Viewer", NULL, NULL);

    if (!gameWindow)
    {
        glfwTerminate();
        glfwSwapInterval(1);
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

    LevelDataClient::requestLevel(0);

    while (!glfwWindowShouldClose(gameWindow))
    {
        this->processInput();
        this->update(0.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->render();

        glfwSwapBuffers(gameWindow);

        glfwPollEvents();
    }

    glfwDestroyWindow(this->gameWindow);
    glfwTerminate();
}

void Game::processInput()
{
}

void Game::update(float deltaTime)
{
    GameObjectManager::getInstance()->update(1.0f/60.0f);
}

void Game::render()
{
    UIManager::getInstance()->drawAllUI();
    GameObjectManager::getInstance()->render(this->shader->GetShaderProgram());
}
