#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "glad/glad.h"

class RawMesh
{
public:
    using String = std::string;

    RawMesh(String meshFilePath, int id, size_t chunkSize);
    ~RawMesh();

    int getId();
    std::string getName();
    const std::vector<GLfloat>& getData(int chunkId);
    std::vector<GLfloat> getAllChunkedData();

private:
    String name;
    int id = 0;
    std::vector<GLfloat> vertexData;
    std::unordered_map<int, std::vector<GLfloat>> chunkedTable;

    void chunkMeshData(size_t chunkSize);
};

