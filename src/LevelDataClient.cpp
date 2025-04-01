#include "LevelDataClient.h"

LevelDataClient::LevelDataClient(std::shared_ptr<grpc::ChannelInterface> channel)
{
}

LevelData LevelDataClient::GetLevelData(int32_t levelId)
{
	return LevelData();
}

MeshTable LevelDataClient::GetMeshData(int32_t levelId)
{
	return MeshTable();
}

TextureTable LevelDataClient::GetTextureData(int32_t levelId)
{
	return TextureTable();
}

MeshDataChunk LevelDataClient::GetMeshDataChunk(int32_t levelId, std::string meshName, int32_t chunkId)
{
	return MeshDataChunk();
}

TextureDataChunk LevelDataClient::GetTextureDataChunk(int32_t levelId, std::string textureName, int32_t chunkId)
{
	return TextureDataChunk();
}

void LevelDataClient::runClient()
{
}
