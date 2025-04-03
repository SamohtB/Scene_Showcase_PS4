#pragma once
#include <cstdint>
#include <vector>
#include "LevelData.pb.h"
#include "LevelData.grpc.pb.h"
#include "IETThread.h"

class LevelDataServer final : public SceneDataService::Service, public IETThread 
{
public:
	grpc::Status GetLevelData(grpc::ServerContext* context, const LevelRequest* request, LevelData* response);
	grpc::Status GetMeshData(grpc::ServerContext* context, const MeshRequest* request, grpc::ServerWriter<MeshTable>* writer);
	grpc::Status GetTextureData(grpc::ServerContext* context, const TextureRequest* request, grpc::ServerWriter<TextureTable>* writer);
	grpc::Status GetMeshDataChunk(grpc::ServerContext* context, const MeshChunkRequest* request, MeshDataChunk* response) override;
	grpc::Status GetTextureDataChunk(grpc::ServerContext* context, const TextureChunkRequest* request, TextureDataChunk* response) override;
	static void RunServer(uint16_t port);

	void run() override;
};