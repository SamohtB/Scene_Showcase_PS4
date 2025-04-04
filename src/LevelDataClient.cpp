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

void LevelDataClient::setSceneIdRequest(int levelId)
{
    this->sceneId = levelId;
}

void LevelDataClient::onStartTask()
{
    LevelData objData = this->GetLevelData(this->sceneId);

    for(int i = 0; i < objData.objectlist().size(); i++)
    {
        ObjectData data = objData.objectlist(i);
        std::string meshName = data.meshreference();
        std::string textureName = data.texturereference();

        int meshChunkCount = 0;
        int textureChunkCount = 0;
        int meshCount = 0;
        bool isLastMeshChunk = false;
        bool isLastTextureChunk = false;
        std::vector<GLfloat> completeMeshData;
        std::vector<uint8_t> completeTextureData;
        int textureWidth = 0;
        int textureHeight = 0;
        int textureChannel = 0;

        do 
        {
            MeshDataChunk chunk = this->GetMeshDataChunk(this->sceneId, meshName, meshChunkCount);

            std::vector<GLfloat> chunkData(reinterpret_cast<const GLfloat*>(chunk.datachunk().data()),
                reinterpret_cast<const GLfloat*>(chunk.datachunk().data() + chunk.datachunk().size()));

            completeMeshData.insert(completeMeshData.end(), chunkData.begin(), chunkData.end());

            Debug::Log(std::to_string(meshChunkCount) + "/" + std::to_string(chunk.maxchunknumber()) + " \n");

            isLastMeshChunk = chunk.islastchunk();
            meshChunkCount++;

        } while (!isLastMeshChunk);

        do
        {
            TextureDataChunk chunk = this->GetTextureDataChunk(this->sceneId, textureName, textureChunkCount);

            std::vector<uint8_t> chunkData(chunk.datachunk().begin(), chunk.datachunk().end());
            completeTextureData.insert(completeTextureData.end(), chunkData.begin(), chunkData.end());

            Debug::Log(std::to_string(textureChunkCount) +"/" + std::to_string(chunk.maxchunknumber()) + " \n");

            isLastTextureChunk = chunk.islastchunk();
            textureChunkCount++;

            if (textureWidth == 0 && textureHeight == 0 && textureChannel == 0)
            {
                textureWidth = chunk.width();
                textureHeight = chunk.height();
                textureChannel = chunk.channel();
            }

        } while (!isLastTextureChunk);

        GameObjectManager::getInstance()->storeMeshData(this->sceneId, i, completeMeshData);
        GameObjectManager::getInstance()->storeTextureData(this->sceneId, i, completeTextureData, textureWidth, textureHeight);
        GameObjectManager::getInstance()->storeLevelData(this->sceneId, data);
    }
}


