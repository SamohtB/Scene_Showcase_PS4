#pragma once
#include <memory>
#include "RawMesh.h"

class Mesh 
{
public:
    Mesh(RawMesh* rawMesh);
    ~Mesh();

    void draw();

    void initializeMesh();

private:
    GLuint VAO, VBO;
    RawMesh* rawMesh;
};

