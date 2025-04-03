#include "LevelDataClient.h"
#include <grpcpp/create_channel.h>
#include "Debug.h"

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

    std::unique_ptr<grpc::ClientReader<TextureData>> reader(stub_->GetTextureData(&context, request));
    TextureTable table;
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
    LevelData reply = levelData.GetLevelData(sceneIndex);

    Debug::Log("Objects received in Level " + std::to_string(sceneIndex) + ":" + "\n");

    for (const auto& object : reply.objectlist())
    {
        Debug::Log("- " + object.name() + "\n");
    }
}
