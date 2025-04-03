#pragma once
#include <string>
#include "LevelData.grpc.pb.h" 

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class LevelDataClient
{
public:
	LevelDataClient(std::shared_ptr<grpc::ChannelInterface> channel);
	LevelData GetLevelData(int32_t levelId);
	MeshTable GetMeshData(int32_t levelId);
	TextureTable GetTextureData(int32_t levelId);
	MeshDataChunk GetMeshDataChunk(int32_t levelId, std::string meshName, int32_t chunkId);
	TextureDataChunk GetTextureDataChunk(int32_t levelId, std::string textureName, int32_t chunkId);

	static void runClient();

private:
	std::unique_ptr<SceneDataService::Stub> stub_;
};