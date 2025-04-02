#include "ServerMain.h"

#include "LevelDataServer.h"
#include "GameObjectManager.h"
#include "Model.h"

ServerMain::ServerMain()
{
	this->serverService = new LevelDataServer();

	/* Add Scenes and Objects */
	GameObjectManager::initialize();

	std::shared_ptr<Model> model = std::make_shared<Model>("Test Model 1", "Models/spot.obj", "Models/bricks2.jpg");
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(0, model);

	model = std::make_shared<Model>("Test Model 2", "Models/spot.obj", "Models/Wall.jpg");
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(1, model);

	model = std::make_shared<Model>("Test Model 3", "Models/spot.obj", "Models/Circuit.jpg");
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(2, model);

	model = std::make_shared<Model>("Test Model 4", "Models/spot.obj", "Models/Shale.jpg");
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(3, model);

	model = std::make_shared<Model>("Test Model 5", "Models/spot.obj", "Models/Crate.jpg");
	model.get()->getTransform()->move(0.0f, 0.0f, -5.0f);
	GameObjectManager::getInstance()->addGameObject(4, model);
}

ServerMain::~ServerMain()
{
	GameObjectManager::getInstance()->clearAllObjects();
}

void ServerMain::run()
{
	this->serverService->start();

	while (true)
	{
		IETThread::sleep(100);
	}
}
