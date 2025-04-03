#include "LevelDataClient.h"
#include <grpcpp/create_channel.h>

LevelDataClient::LevelDataClient(std::shared_ptr<grpc::ChannelInterface> channel)
{
	this->stub_ = SceneDataService::NewStub(channel);
}

LevelData LevelDataClient::GetLevelData(int32_t levelId)
{
    LevelRequest request;
    request.set_levelid(levelId);
    grpc::ClientContext context;

    std::chrono::time_point deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(2000);
    context.set_deadline(deadline);

    std::unique_ptr<grpc::ClientReader<LevelData>> reader(stub_->GetLevelData(&context, request));

    LevelData compiledResponse;
    LevelData tempResponse;

    while (reader->Read(&tempResponse))
    {
        for (const auto& object : tempResponse.objectlist())
        {
            ObjectData* newObject = compiledResponse.add_objectlist();
            *newObject = object;
        }
    }

    Status status = reader->Finish();
    if (!status.ok())
    {
        std::cerr << "GetLevelData RPC failed: " << status.error_message() << std::endl;
    }

    return compiledResponse;
}

//MeshTable LevelDataClient::GetMeshData(int32_t levelId)
//{
//	return MeshTable();
//}
//
//TextureTable LevelDataClient::GetTextureData(int32_t levelId)
//{
//	return TextureTable();
//}
//
//MeshDataChunk LevelDataClient::GetMeshDataChunk(int32_t levelId, std::string meshName, int32_t chunkId)
//{
//	return MeshDataChunk();
//}
//
//TextureDataChunk LevelDataClient::GetTextureDataChunk(int32_t levelId, std::string textureName, int32_t chunkId)
//{
//	return TextureDataChunk();
//}

void LevelDataClient::runClient()
{
    //LevelDataClient levelData(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    //int sceneIndex = 0;
    //LevelData reply = levelData.GetLevelData(sceneIndex);

    //std::cout << "Objects received in Level " << sceneIndex << ":" << std::endl;
    //for (const auto& object : reply.objectlist())
    //{
    //    std::cout << " - " << object.name() << std::endl;
    //}
}
