 #include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Debug.h"

Mesh::Mesh(String meshFilePath)
{
    compileVertexData(meshFilePath);
    setupMesh();
}

void Mesh::compileVertexData(String meshFilePath)
{
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error, warning;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &error, &warning, meshFilePath.c_str());

    if (!success) {
        Debug::Log("Failed to load OBJ: " + error);
        return;
    }
    if (!warning.empty()) {
        Debug::Log("OBJ Warning: " + warning);
    }

    std::vector<glm::vec3> tangents;

    /* get vertices */
    for (size_t s = 0; s < shapes.size(); s++)
    {
        for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++)
        {
            tinyobj::index_t vData = shapes[s].mesh.indices[i];

            vertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
            vertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
            vertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);
            vertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
            vertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
        }
    }
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    /* GL_ARRAY_BUFFER, Size of Whole Vertex Data, Vertex Data point, GL_STATIC_DRAW*/
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    /* position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    /* uv */
    GLuint uvPtr = 3 * sizeof(GLfloat);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)uvPtr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexData.size() / 5));
    glBindVertexArray(0);
}
