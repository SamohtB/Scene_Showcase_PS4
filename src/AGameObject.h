#pragma once
#include <string>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Texture;
class Mesh;

class AGameObject
{
public:
    using String = std::string;
    using TexturePtr = std::shared_ptr<Texture>;
    using MeshPtr = std::shared_ptr<Mesh>;

    AGameObject(String name);
    virtual ~AGameObject();

    virtual void update(float deltaTime) = 0;
    virtual void draw(GLuint shaderProgram) = 0;

    std::string getName();
    void setName(String name);

protected:
    String name;

private:
    friend class GameObjectManager;
};

