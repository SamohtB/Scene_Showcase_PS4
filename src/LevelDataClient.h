#pragma once
#include <string>
#include "LevelData.grpc.pb.h" 
#include "IWorkerAction.h"
#include <glad/glad.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class LevelDataClient : public IWorkerAction
{
public:
	LevelDataClient(std::shared_ptr<grpc::ChannelInterface> channel);
	LevelData GetLevelData(int32_t levelId);
	MeshDataChunk GetMeshDataChunk(int32_t levelId, std::string meshName, int32_t chunkId);
	TextureDataChunk GetTextureDataChunk(int32_t levelId, std::string textureName, int32_t chunkId);

	void setSceneIdRequest(int levelId);

private:
	std::unique_ptr<SceneDataService::Stub> stub_;
	int sceneId = 0;


	// Inherited via IWorkerAction
	void onStartTask() override;

	std::mutex dataMutex;
};