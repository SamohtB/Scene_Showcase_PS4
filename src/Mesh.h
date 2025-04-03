#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <memory>

class Mesh 
{
public:
    using String = std::string;

    Mesh(String meshFilePath, int id, bool isRendered);
    ~Mesh();

    void draw();

    int getId();
    std::string getName();
    const std::vector<GLfloat>& getVertexData();

private:
    GLuint VAO, VBO = 0;
    std::vector<GLfloat> vertexData;
    int id = 0;
    String name;
    bool isRendered = true;

    void setupMesh();
    void compileVertexData(String meshFilePath);
};

