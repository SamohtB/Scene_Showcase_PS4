#include "GameObjectManager.h"
#include "AGameObject.h"

GameObjectManager* GameObjectManager::sharedInstance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
    return sharedInstance;
}

void GameObjectManager::initialize()
{
    sharedInstance = new GameObjectManager();
}

void GameObjectManager::destroy()
{
    delete sharedInstance;
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
    clearAllObjects();
}

GameObjectManager::ObjectPtr GameObjectManager::findObjectByName(String name)
{
    for (auto& obj : objectList)
    {
        if (obj->getName() == name)
        {
            return obj;
        }   
    }
    return nullptr;
}

GameObjectManager::ObjectList GameObjectManager::getAllObjects()
{
    return this->objectList;
}

void GameObjectManager::addGameObject(const int& sceneId, ObjectPtr gameObject)
{
    if (!gameObject) return;

    this->sceneTable[sceneId].push_back(gameObject);
    this->objectList.push_back(gameObject);
}

void GameObjectManager::deleteObject(const int& sceneId, ObjectPtr gameObject)
{
    if(!gameObject) return;

    auto& sceneObjects = sceneTable[sceneId];
    sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), gameObject), sceneObjects.end());
    objectList.erase(std::remove(objectList.begin(), objectList.end(), gameObject), objectList.end());
}

void GameObjectManager::deleteObjectByName(const int& sceneId, String name)
{
    ObjectPtr object = findObjectByName(name);

    if (object)
    {
        deleteObject(sceneId, object);
    }
}

void GameObjectManager::clearAllObjects()
{
    for (auto& [sceneId, objects] : sceneTable)
    {
        objects.clear();
    }

    objectList.clear();
    sceneTable.clear();
}

void GameObjectManager::processInput()
{
}

void GameObjectManager::update(float deltaTime)
{
    for (auto& obj : objectList)
    {
        obj->update(deltaTime);
    }
}

void GameObjectManager::render(GLuint shaderProgram)
{
    for (auto& obj : objectList)
    {
        obj->draw(shaderProgram);
    }
}


