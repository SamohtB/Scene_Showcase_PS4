#pragma once
class SceneManager
{
public:
    using MeshList

    static SceneManager* getInstance();
    static void initialize();
    static void destroy();

    ObjectPtr findObjectByName(String name);
    ObjectList getAllObjects();

    void addGameObject(const int& sceneId, ObjectPtr gameObject);
    void deleteObject(const int& sceneId, ObjectPtr game_object);
    void deleteObjectByName(const int& sceneId, String name);
    void clearAllObjects();

    ObjectList getScene(int sceneId);

private:
    SceneManager();
    ~SceneManager();
    SceneManager(SceneManager const&) {}
    SceneManager& operator=(SceneManager const&) {}

    static SceneManager* sharedInstance;
};

