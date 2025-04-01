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

    String getName();
    void setName(String name);
    TexturePtr getTexture();
    void setTexture(TexturePtr texture);
    MeshPtr getMesh();
    void setMesh(MeshPtr mesh);

protected:
    String name;
    TexturePtr texture;
    MeshPtr mesh;

private:
    friend class GameObjectManager;
};

