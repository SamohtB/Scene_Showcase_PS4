#include "LevelDataClient.h"
#include <grpcpp/create_channel.h>
#include "Debug.h"
#include "GameObjectManager.h"
#include "Model.h"

LevelDataClient::LevelDataClient(std::shared_ptr<grpc::ChannelInterface> channel)
{
	this->stub_ = SceneDataService::NewStub(channel);
}

LevelData LevelDataClient::GetLevelData(int32_t levelId)
{
    LevelRequest request;
    request.set_levelid(levelId);
    LevelData reply;
    grpc::ClientContext context;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    grpc::Status status = stub_->GetLevelData(&context, request, &reply);

    if (!status.ok())
    {
        Debug::Log(status.error_code() + ": " + status.error_message() + "\n");
    }

    return reply;
}

MeshTable LevelDataClient::GetMeshData(int32_t levelId)
{
    MeshRequest request;
    request.set_levelid(levelId);
    grpc::ClientContext context;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    std::unique_ptr<grpc::ClientReader<MeshTable>> reader(stub_->GetMeshData(&context, request));

    MeshTable table;

    while (reader->Read(&table))
    {
        for (const auto& pair : table.meshdatatable())
        {
            int meshId = pair.first;
            const MeshData& meshData = pair.second;

            for (const MeshDataChunk& chunk : meshData.meshdatachunk())
            {
                Debug::Log("Mesh ID: " + std::to_string(meshId) +
                    ", Name: " + chunk.name() +
                    ", Chunk: " + std::to_string(chunk.chunknumber()) +
                    "/" + std::to_string(chunk.maxchunknumber()) +
                    (chunk.islastchunk() ? " [Last Chunk]" : "") +
                    "\n");
            }
        }
    }

	return table;
}

TextureTable LevelDataClient::GetTextureData(int32_t levelId)
{
    TextureRequest request;
    request.set_levelid(levelId);
    grpc::ClientContext context;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    std::unique_ptr<grpc::ClientReader<TextureTable>> reader(stub_->GetTextureData(&context, request));
    TextureTable table;

    while (reader->Read(&table))
    {
        for (const auto& pair : table.texturedatatable())
        {
            int textureId = pair.first;
            const TextureData& textureData = pair.second;

            for (const TextureDataChunk& chunk : textureData.texturedatachunk())
            {
                Debug::Log("Texture ID: " + std::to_string(textureId) +
                    ", Name: " + chunk.name() +
                    ", Chunk: " + std::to_string(chunk.chunknumber()) +
                    "/" + std::to_string(chunk.maxchunknumber()) +
                    ", Size: " + std::to_string(chunk.width()) + "x" + std::to_string(chunk.height()) +
                    (chunk.islastchunk() ? " [Last Chunk]" : "") +
                    "\n");
            }
        }
    }
	return table;
}

MeshDataChunk LevelDataClient::GetMeshDataChunk(int32_t levelId, std::string meshName, int32_t chunkId)
{
    MeshChunkRequest request;
    request.set_levelid(levelId);
    request.set_name(meshName);
    request.set_chunknumber(chunkId);
    grpc::ClientContext context;
    MeshDataChunk response;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    grpc::Status status = stub_->GetMeshDataChunk(&context, request, &response);

	return response;
}

TextureDataChunk LevelDataClient::GetTextureDataChunk(int32_t levelId, std::string textureName, int32_t chunkId)
{
    TextureChunkRequest request;
    request.set_levelid(levelId);
    request.set_name(textureName);
    request.set_chunknumber(chunkId);
    grpc::ClientContext context;
    TextureDataChunk response;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    grpc::Status status = stub_->GetTextureDataChunk(&context, request, &response);

    return response;
}

void LevelDataClient::runClient()
{
    LevelDataClient levelData(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    int sceneIndex = 0;
    MeshTable reply = levelData.GetMeshData(sceneIndex);
}

void LevelDataClient::requestLevel(int levelId)
{
    LevelDataClient levelData(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    LevelData objData = levelData.GetLevelData(levelId);
    MeshTable meshTable = levelData.GetMeshData(levelId);
    TextureTable textureTable = levelData.GetTextureData(levelId);

    // Store mesh data as GLfloat
    std::unordered_map<int, std::vector<GLfloat>> completeMeshDataGL;
    for (const auto& pair : meshTable.meshdatatable())
    {
        int meshId = pair.first;
        const MeshData& meshData = pair.second;

        std::vector<MeshDataChunk> sortedChunks(meshData.meshdatachunk().begin(), meshData.meshdatachunk().end());
        std::sort(sortedChunks.begin(), sortedChunks.end(),
            [](const MeshDataChunk& a, const MeshDataChunk& b) { return a.chunknumber() < b.chunknumber(); });

        std::vector<uint8_t> meshBuffer;
        for (const MeshDataChunk& chunk : sortedChunks)
        {
            meshBuffer.insert(meshBuffer.end(), chunk.datachunk().begin(), chunk.datachunk().end());
        }

        // Convert uint8_t buffer to GLfloat
        std::vector<GLfloat> meshDataGL;
        size_t numFloats = meshBuffer.size() / sizeof(GLfloat);
        meshDataGL.reserve(numFloats);

        for (size_t i = 0; i < numFloats; ++i)
        {
            GLfloat value;
            std::memcpy(&value, &meshBuffer[i * sizeof(GLfloat)], sizeof(GLfloat));
            meshDataGL.push_back(value);
        }

        completeMeshDataGL[meshId] = std::move(meshDataGL);
    }

    // Store texture data as uint8_t
    std::unordered_map<int, std::vector<uint8_t>> completeTextureData;
    std::unordered_map<int, std::pair<int, int>> textureDimensions;
    for (const auto& pair : textureTable.texturedatatable())
    {
        int textureId = pair.first;
        const TextureData& textureData = pair.second;

        std::vector<TextureDataChunk> sortedChunks(textureData.texturedatachunk().begin(), textureData.texturedatachunk().end());
        std::sort(sortedChunks.begin(), sortedChunks.end(),
            [](const TextureDataChunk& a, const TextureDataChunk& b) { return a.chunknumber() < b.chunknumber(); });

        std::vector<uint8_t> textureBuffer;
        for (const TextureDataChunk& chunk : sortedChunks)
        {
            textureBuffer.insert(textureBuffer.end(), chunk.datachunk().begin(), chunk.datachunk().end());
            textureDimensions[textureId] = { chunk.width(), chunk.height() };
        }
        completeTextureData[textureId] = std::move(textureBuffer);
    }

    // Create and add game objects
    for (const auto& obj : objData.objectlist())
    {
        int meshId = obj.meshreference();
        int textureId = obj.texturereference();

        // Access mesh data from completeMeshDataGL
        auto meshIt = completeMeshDataGL.find(meshId);
        auto textureIt = completeTextureData.find(textureId);

        if (meshIt == completeMeshDataGL.end() || textureIt == completeTextureData.end())
        {
            Debug::Log("Skipping model: " + obj.name() + " due to missing mesh or texture\n");
            continue;
        }

        // Create mesh from the data
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(meshIt->second, meshId, true);

        // Get texture dimensions and create texture
        auto [width, height] = textureDimensions[textureId];
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureIt->second, textureId, true, width, height, 3);

        // Create model and set its transform
        std::shared_ptr<Model> model = std::make_shared<Model>(obj.name(), mesh, texture);
        Transform* transform = model->getTransform();

        transform->setPosition(obj.position().x(), obj.position().y(), obj.position().z());
        transform->setRotation(obj.rotation().x(), obj.rotation().y(), obj.rotation().z());
        transform->setScale(obj.scale().x(), obj.scale().y(), obj.scale().z());

        // Add the model as a game object
        GameObjectManager::getInstance()->addGameObject(levelId, std::static_pointer_cast<AGameObject>(model));
        Debug::Log("Added model: " + obj.name() + "\n");
    }
}


