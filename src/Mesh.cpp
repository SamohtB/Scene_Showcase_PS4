 #include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <filesystem>
#include "Debug.h"

Mesh::Mesh(RawMesh* mesh) : rawMesh(mesh), VAO(0), VBO(0)
{
    initializeMesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void Mesh::initializeMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    /* GL_ARRAY_BUFFER, Size of Whole Vertex Data, Vertex Data point, GL_STATIC_DRAW*/
    glBufferData(GL_ARRAY_BUFFER, rawMesh->getAllChunkedData().size() * sizeof(GLfloat), rawMesh->getAllChunkedData().data(), GL_STATIC_DRAW);

    /* position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    /* uv */
    GLuint uvPtr = 3 * sizeof(GLfloat);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)uvPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(rawMesh->getAllChunkedData().size() / 5));
    glBindVertexArray(0);
}
