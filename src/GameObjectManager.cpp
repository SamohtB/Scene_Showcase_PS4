#include "GameObjectManager.h"
#include "AGameObject.h"
#include "Debug.h"
#include "Model.h"
#include "Transform.h"

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
    this->dataMutex = new GenericMonitor();
    this->dataMutex->open();
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

GameObjectManager::ObjectList GameObjectManager::getScene(int sceneId)
{
    return this->sceneTable[sceneId];
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

void GameObjectManager::storeLevelData(int sceneId, const ObjectData& data)
{
    this->dataMutex->tryEnter();
    levelDataMap[sceneId] = data;
    this->dataMutex->reportExit();
}

void GameObjectManager::storeMeshData(int sceneId, int meshId, const std::vector<GLfloat>& meshData)
{
    this->dataMutex->tryEnter();
    meshDataMap[sceneId][meshId] = meshData;
    this->dataMutex->reportExit();
}

void GameObjectManager::storeTextureData(int sceneId, int textureId, const std::vector<uint8_t>& textureData, int width, int height)
{
    this->dataMutex->tryEnter();
    textureDataMap[sceneId][textureId] = { textureData, width, height };
    this->dataMutex->reportExit();
}

float getRandomPosition()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-4.0f, 4.0f);
    return dis(gen);
}

void GameObjectManager::processPendingData()
{
    int count = 0;

    for (const auto& pair : levelDataMap)
    {
        int sceneId = pair.first;
        ObjectData data = pair.second;

		// Process the mesh data for the current object
		std::vector<GLfloat> vertexData = this->meshDataMap[sceneId][count];
		auto mesh = std::make_shared<Mesh>(vertexData, count, true);

		// Process the texture data for the current object
        std::vector<uint8_t> textureData = std::get<0>(this->textureDataMap[sceneId][count]);
		int textureWidth = std::get<1>(this->textureDataMap[sceneId][count]);
		int textureHeight = std::get<2>(this->textureDataMap[sceneId][count]);
		auto texture = std::make_shared<Texture>(textureData, count, true, textureWidth, textureHeight, 3);

		// Create the model using mesh and texture
		std::shared_ptr<Model> model = std::make_shared<Model>(data.name(), mesh, texture);
		Transform* transform = model->getTransform();

		transform->setPosition(getRandomPosition(), getRandomPosition(), getRandomPosition());
		transform->setRotation(0.0f, 0.0f, 0.0f);
		transform->setScale(1.0f, 1.0f, 1.0f);

		GameObjectManager::getInstance()->addGameObject(sceneId, std::static_pointer_cast<AGameObject>(model));
		Debug::Log("Added model: " + data.name() + " to scene " + std::to_string(sceneId));
    }

    /* clear data */
    levelDataMap.clear();
    for (auto& sceneMeshData : meshDataMap)
    {
        sceneMeshData.second.clear();
    }
    meshDataMap.clear();

    for (auto& sceneTextureData : textureDataMap)
    {
        sceneTextureData.second.clear();
    }
    textureDataMap.clear();
}


