#include "ServerMain.h"

#include "LevelDataServer.h"
#include "GameObjectManager.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"

#include <GLFW/glfw3.h>

ServerMain::ServerMain()
{
	this->serverService = new LevelDataServer();


	std::shared_ptr<RawMesh> cowMesh = std::make_shared<RawMesh>("Models/spot.obj", 0, false);
	std::shared_ptr<Texture> brick = std::make_shared<Texture>("Models/bricks2.jpg", 0, false);
	std::shared_ptr<Texture> wall = std::make_shared<Texture>("Models/Wall.jpg", 1, false);
	std::shared_ptr<Texture> circuit = std::make_shared<Texture>("Models/Circuit.jpg", 2, false);
	std::shared_ptr<Texture> shale = std::make_shared<Texture>("Models/Shale.jpg", 3, false);
	std::shared_ptr<Texture> crate = std::make_shared<Texture>("Models/Crate.jpg", 4, false);

	std::shared_ptr<Model> model = std::make_shared<Model>("Test Model 1", cowMesh, brick);
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(0, model);

	model = std::make_shared<Model>("Test Model 2", cowMesh, wall);
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(1, model);

	model = std::make_shared<Model>("Test Model 3", cowMesh, circuit);
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(2, model);

	model = std::make_shared<Model>("Test Model 4", cowMesh, shale);
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(3, model);

	model = std::make_shared<Model>("Test Model 5", cowMesh, crate);
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(4, model);
}

ServerMain::~ServerMain()
{

}

void ServerMain::run()
{
	this->serverService->start();

	while (true)
	{
		IETThread::sleep(100);
	}
}
