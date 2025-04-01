#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <exception>
#include <memory>

#include "AGameObject.h"

class AGameObject;
class Transform;

class GameObjectManager
{
public:
    using String = std::string;
    using ObjectPtr = std::shared_ptr<AGameObject>;
    using ObjectList = std::vector<ObjectPtr>;
    using SceneTable = std::unordered_map<int, ObjectList>;

    static GameObjectManager* getInstance();
    static void initialize();
    static void destroy();

    ObjectPtr findObjectByName(String name);
    ObjectList getAllObjects();
    
    void addGameObject(const int& sceneId, ObjectPtr gameObject);
    void deleteObject(const int& sceneId, ObjectPtr game_object);
    void deleteObjectByName(const int& sceneId, String name);
    void clearAllObjects();

    void processInput();
    void update(float deltaTime);
    void render(GLuint shaderProgram);

private:
    GameObjectManager();
    ~GameObjectManager();
    GameObjectManager(GameObjectManager const&) {}
    GameObjectManager& operator=(GameObjectManager const&) {}

    static GameObjectManager* sharedInstance;

    ObjectList objectList;
    SceneTable sceneTable;
};

