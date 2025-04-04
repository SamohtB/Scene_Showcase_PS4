#include "Game.h"

#include <stdlib.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "config.h"
#include "Debug.h"

#include "GameObjectManager.h"
#include "UIManager.h"
#include "EngineTime.h"
#include "ClientRequestManager.h"

#include "Model.h"

Game::Game()
{
    initializeWindow();
    this->shader = new Shader("Shaders/Shader.vert", "Shaders/Shader.frag");

    EngineTime::initialize();
    EngineTime::setFrameTime(60);
    Debug::initialize();
    ClientRequestManager::initialize();
    UIManager::initialize(this->gameWindow);
    GameObjectManager::initialize();
}

Game::~Game()
{
    GameObjectManager::destroy();
    UIManager::destroy();
    ClientRequestManager::destroy();
    Debug::destroy();
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

    while (!glfwWindowShouldClose(gameWindow))
    {
        EngineTime::LogFrameStart();

        this->processInput();
        this->update(static_cast<float>(EngineTime::getDeltaTime()));
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->render();

        glfwSwapBuffers(gameWindow);
        glfwPollEvents();

        EngineTime::LogFrameEnd();

        frameCount++;
        totalTime += EngineTime::getDeltaTime();

        if (totalTime >= updateInterval)
        {
            double averageFPS = frameCount / totalTime;
            UIManager::getInstance()->updateFPS(averageFPS);

            frameCount = 0;
            totalTime = 0.0;
        }
    }

    glfwDestroyWindow(this->gameWindow);
    glfwTerminate();
}

void Game::processInput()
{
}

void Game::update(float deltaTime)
{
    GameObjectManager::getInstance()->processPendingData();
    GameObjectManager::getInstance()->update(deltaTime);
}

void Game::render()
{
    UIManager::getInstance()->drawAllUI();
    GameObjectManager::getInstance()->render(this->shader->GetShaderProgram());
}
