#include "AMesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

AMesh::AMesh(std::string strMesh)
{
    LoadMesh(strMesh);

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
}

AMesh::~AMesh()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

GLuint AMesh::getVAO()
{
    return this->VAO;
}

GLuint AMesh::getVBO()
{
    return this->VBO;
}

std::vector<GLfloat> AMesh::getVertexData()
{
    return this->fullVertexData;
}

void AMesh::LoadMesh(std::string strMesh)
{
    std::string path = strMesh;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string error, warning;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &error,
        &warning,
        path.c_str());

    if (!success) {
        std::cerr << "Failed to load OBJ: " << error << std::endl;
        return;
    }
    if (!warning.empty()) {
        std::cerr << "OBJ Warning: " << warning << std::endl;
    }

    std::vector<glm::vec3> tangents;

    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i += 3)
    {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[vData1.vertex_index * 3 + 1],
            attributes.vertices[vData1.vertex_index * 3 + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[vData2.vertex_index * 3 + 1],
            attributes.vertices[vData2.vertex_index * 3 + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[vData3.vertex_index * 3 + 1],
            attributes.vertices[vData3.vertex_index * 3 + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[vData1.texcoord_index * 2 + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[vData2.texcoord_index * 2 + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[vData3.texcoord_index * 2 + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);
    }

    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);      // X pos
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);  // Y pos
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);  // Z pos

        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);       // R nor
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);   // S nor
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);   // T nor

        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);       // U tex
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);   // V tex

        this->fullVertexData.push_back(tangents[i].x);
        this->fullVertexData.push_back(tangents[i].y);
        this->fullVertexData.push_back(tangents[i].z);
    }
}
