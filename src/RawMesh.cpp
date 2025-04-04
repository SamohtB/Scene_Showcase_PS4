#include "RawMesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Debug.h"

#include <filesystem>

#include <glm/glm.hpp>

RawMesh::RawMesh(String meshFilePath, int id, size_t chunkSize)
{
	name = std::filesystem::path(meshFilePath).stem().string();

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

    for (size_t s = 0; s < shapes.size(); s++)
    {
        for (size_t i = 0; i < shapes[s].mesh.indices.size(); i++)
        {
            tinyobj::index_t vData = shapes[s].mesh.indices[i];

            vertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
            vertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 1]);
            vertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);

            if (!attributes.texcoords.empty() && vData.texcoord_index >= 0)
            {
                vertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
                vertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
            }
            else
            {
                vertexData.push_back(0.0f);
                vertexData.push_back(0.0f);
            }
        }
    }

    chunkMeshData(chunkSize);
}

RawMesh::~RawMesh()
{
}

int RawMesh::getId()
{
	return this->id;
}

std::string RawMesh::getName()
{
	return this->name;
}

const std::vector<GLfloat>& RawMesh::getData(int chunkId)
{
    return this->chunkedTable[chunkId];
}

std::vector<GLfloat> RawMesh::getAllChunkedData()
{
    std::vector<GLfloat> allData;

    for (const auto& chunk : chunkedTable)
    {
        allData.insert(allData.end(), chunk.second.begin(), chunk.second.end());
    }

    return allData;
}

void RawMesh::chunkMeshData(size_t chunkSize)
{
    chunkedTable.clear();

    size_t totalSize = vertexData.size() * sizeof(GLfloat);
    size_t totalChunks = (totalSize + chunkSize - 1) / chunkSize;

    for (size_t chunkNumber = 0; chunkNumber < totalChunks; chunkNumber++) 
    {
        size_t start = chunkNumber * chunkSize;
        size_t end = std::min(start + chunkSize, totalSize);
        size_t dataSize = end - start;

        std::vector<GLfloat> chunkData(dataSize / sizeof(GLfloat));
        std::memcpy(chunkData.data(), &vertexData[start / sizeof(GLfloat)], dataSize);

        chunkedTable[chunkNumber] = std::move(chunkData);
    }
}
