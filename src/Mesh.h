#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <memory>

using String = std::string;

class Mesh 
{
public:
    Mesh(String meshFilePath);
    
    ~Mesh();

    void draw();
private:
    GLuint VAO, VBO;
    std::vector<GLfloat> vertexData;

    void setupMesh();
    void compileVertexData(String meshFilePath);
};

