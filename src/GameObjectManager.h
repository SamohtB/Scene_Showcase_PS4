#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <exception>
#include <memory>
#include <mutex>

#include "AGameObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "leveldata.pb.h"
#include "GenericMonitor.h"


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

    ObjectList getScene(int sceneId);

    void processInput();
    void update(float deltaTime);
    void render(GLuint shaderProgram);

    void storeLevelData(int sceneId, const ObjectData& data);
    void storeMeshData(int sceneId, int meshId, const std::vector<GLfloat>& meshData);
    void storeTextureData(int sceneId, int textureId, const std::vector<uint8_t>& textureData, int width, int height);
    void processPendingData();

private:
    GameObjectManager();
    ~GameObjectManager();
    GameObjectManager(GameObjectManager const&) {}
    GameObjectManager& operator=(GameObjectManager const&) {}

    static GameObjectManager* sharedInstance;

    ObjectList objectList;
    SceneTable sceneTable;

    std::unordered_map<int, ObjectData> levelDataMap;
    std::unordered_map<int, std::unordered_map<int, std::vector<GLfloat>>> meshDataMap;
    std::unordered_map<int, std::unordered_map<int, std::tuple<std::vector<uint8_t>, int, int>>> textureDataMap;

    GenericMonitor* dataMutex = nullptr;
};

