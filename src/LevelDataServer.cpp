#include "LevelDataServer.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "GameObjectManager.h"
#include "Model.h"

grpc::Status LevelDataServer::GetLevelData(grpc::ServerContext* context, const LevelRequest* request, LevelData* response)
{
	/* Get List of GameObjects */
	int levelId = request->levelid();
	GameObjectManager::ObjectList gameObjects = GameObjectManager::getInstance()->getScene(levelId);

	LevelData levelData;

	/* compile gameobject data */
	for (const GameObjectManager::ObjectPtr& obj : gameObjects)
	{
		std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(obj);

		if (model)
		{
			ObjectData* modelData = response->add_objectlist();
			modelData->set_name(model->getName());
			
			int meshId = model->getMesh()->getId();
			int textureId = model->getTexture()->getId();
			modelData->set_meshreference(std::to_string(meshId));
			modelData->set_texturereference(std::to_string(textureId));

		}
	}
	
    return grpc::Status::OK;
}

grpc::Status LevelDataServer::GetMeshDataChunk(grpc::ServerContext* context, const MeshChunkRequest* request, MeshDataChunk* response)
{
	int levelId = request->levelid();
	std::string name = request->name();
	int chunkNumber = request->chunknumber();

	GameObjectManager::ObjectPtr gameObject = GameObjectManager::getInstance()->findObjectByName(name);

	if (!gameObject) 
	{
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Model not found for the requested name.");
	}

	auto model = std::dynamic_pointer_cast<Model>(gameObject);

	std::vector<GLfloat> meshData = model->getMesh()->getVertexData();

	/* Chunking */
	size_t chunkSize = 512;
	size_t totalSize = meshData.size();
	int totalChunks = (totalSize + chunkSize - 1) / chunkSize;

	response->set_name(request->name());

	size_t start = chunkNumber * chunkSize;
	size_t end = std::min(start + chunkSize, totalSize);
	size_t dataSize = end - start;

	response->set_datachunk(reinterpret_cast<const char*>(&meshData[start]), dataSize * sizeof(GLfloat));
	response->set_chunknumber(chunkNumber);
	response->set_maxchunknumber(totalChunks - 1);
	response->set_islastchunk(chunkNumber == totalChunks - 1);
	
	return grpc::Status::OK;
}

grpc::Status LevelDataServer::GetTextureDataChunk(grpc::ServerContext* context, const TextureChunkRequest* request, TextureDataChunk* response)
{
	int levelId = request->levelid();
	std::string name = request->name();
	int chunkNumber = request->chunknumber();

	GameObjectManager::ObjectPtr gameObject = GameObjectManager::getInstance()->findObjectByName(name);

	if (!gameObject)
	{
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Model not found for the requested name.");
	}

	auto model = std::dynamic_pointer_cast<Model>(gameObject);

	if (!model) {
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Model not found for the requested name.");
	}

	std::vector<uint8_t> textureData = model->getTexture()->getTextureData();

	/* chunking */
	size_t chunkSize = 512;
	size_t totalSize = textureData.size();
	int totalChunks = (totalSize + chunkSize - 1) / chunkSize;

	response->set_name(request->name());

	size_t start = chunkNumber * chunkSize;
	size_t end = std::min(start + chunkSize, totalSize);
	size_t dataSize = end - start;

	response->set_datachunk(reinterpret_cast<const char*>(&textureData[start]), dataSize);

	response->set_chunknumber(chunkNumber);
	response->set_maxchunknumber(totalChunks - 1);
	response->set_islastchunk(chunkNumber == totalChunks - 1);
	response->set_width(model->getTexture()->getWidth());
	response->set_height(model->getTexture()->getHeight());
	response->set_channel(model->getTexture()->getcolorChannels());

    return grpc::Status::OK;
}

void LevelDataServer::RunServer(uint16_t port)
{
    std::string serverAddress = absl::StrFormat("localhost:%d", port);
    LevelDataServer service;

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	grpc::ServerBuilder builder;

	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());

	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService(&service);

	// Finally assemble the server.
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << serverAddress << std::endl;
	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}

void LevelDataServer::run()
{
	RunServer(50051);
}
