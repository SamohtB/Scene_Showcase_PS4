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

			Vec3* position = modelData->mutable_position();
			Vec3* rotation = modelData->mutable_rotation();
			Vec3* scale = modelData->mutable_scale();

			Transform* transform = model->getTransform();

			position->set_x(transform->getPosition().x);
			position->set_y(transform->getPosition().y);
			position->set_z(transform->getPosition().z);

			rotation->set_x(transform->getRotation().x);
			rotation->set_y(transform->getRotation().y);
			rotation->set_z(transform->getRotation().z);

			scale->set_x(transform->getScale().x);
			scale->set_y(transform->getScale().y);
			scale->set_z(transform->getScale().z);
			
			int meshId = model->getMesh()->getId();
			int textureId = model->getTexture()->getId();
			modelData->set_meshreference(meshId);
			modelData->set_texturereference(textureId);
		}
	}
	
    return grpc::Status::OK;
}

grpc::Status LevelDataServer::GetMeshData(grpc::ServerContext* context, const MeshRequest* request, grpc::ServerWriter<MeshTable>* writer)
{
	int levelId = request->levelid();
	GameObjectManager::ObjectList gameObjects = GameObjectManager::getInstance()->getScene(levelId);

	std::unordered_map<int, std::shared_ptr<Mesh>> uniqueMeshes;

	for (const auto& obj : gameObjects)
	{
		auto model = std::dynamic_pointer_cast<Model>(obj);
		if (!model) continue;

		std::shared_ptr<Mesh> mesh = model->getMesh();
		int meshId = mesh->getId();

		if (uniqueMeshes.find(meshId) == uniqueMeshes.end())
		{
			uniqueMeshes[meshId] = mesh;
		}
	}

	for (const auto& pair : uniqueMeshes)
	{
		int meshId = pair.first;
		auto mesh = pair.second;
		const std::vector<GLfloat>& meshData = mesh->getVertexData();

		MeshTable meshTable;
		MeshData& meshDataEntry = (*meshTable.mutable_meshdatatable())[meshId];

		/* Chunking */
		size_t chunkSize = 512;
		size_t totalSize = meshData.size();
		int totalChunks = (totalSize + chunkSize - 1) / chunkSize;

		for (int chunkNumber = 0; chunkNumber < totalChunks; ++chunkNumber)
		{
			MeshDataChunk* meshChunk = meshDataEntry.add_meshdatachunk();
			meshChunk->set_name(mesh->getName());

			size_t start = chunkNumber * chunkSize;
			size_t end = std::min(start + chunkSize, totalSize);
			size_t dataSize = end - start;

			meshChunk->set_datachunk(reinterpret_cast<const char*>(&meshData[start]), dataSize * sizeof(GLfloat));
			meshChunk->set_chunknumber(chunkNumber);
			meshChunk->set_maxchunknumber(totalChunks - 1);
			meshChunk->set_islastchunk(chunkNumber == totalChunks - 1);
		}

		if (!writer->Write(meshTable))
		{
			return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to stream mesh data.");
		}
	}

	return grpc::Status::OK;
}


grpc::Status LevelDataServer::GetTextureData(grpc::ServerContext* context, const TextureRequest* request, grpc::ServerWriter<TextureTable>* writer)
{
	int levelId = request->levelid();
	GameObjectManager::ObjectList gameObjects = GameObjectManager::getInstance()->getScene(levelId);

	std::unordered_map<int, std::shared_ptr<Texture>> uniqueTextures;

	for (const auto& obj : gameObjects)
	{
		auto model = std::dynamic_pointer_cast<Model>(obj);
		if (!model) continue;

		std::shared_ptr<Texture> texture(model->getTexture());
		int textureId = texture->getId();

		if (uniqueTextures.find(textureId) == uniqueTextures.end())
		{
			uniqueTextures[textureId] = texture;
		}
	}

	for (const auto& pair : uniqueTextures)
	{
		int textureId = pair.first;
		auto texture = pair.second;

		std::vector<unsigned char> textureData = texture->getTextureData();
		int width, height, channels = 3; /* current textures are jpg */

		glBindTexture(GL_TEXTURE_2D, textureId);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		glBindTexture(GL_TEXTURE_2D, 0);

		size_t totalSize = width * height * channels;

		if (totalSize != textureData.size()) 
		{
			return grpc::Status(grpc::StatusCode::INTERNAL, "Texture data size mismatch.");
		}

		TextureTable textureTable;
		TextureData& textureDataEntry = (*textureTable.mutable_texturedatatable())[textureId];

		/* Chunking */
		size_t chunkSize = 512;
		int totalChunks = (totalSize + chunkSize - 1) / chunkSize;

		for (int chunkNumber = 0; chunkNumber < totalChunks; ++chunkNumber)
		{
			TextureDataChunk* textureChunk = textureDataEntry.add_texturedatachunk();
			textureChunk->set_name(texture->getName());

			size_t start = chunkNumber * chunkSize;
			size_t end = std::min(start + chunkSize, totalSize);
			size_t dataSize = end - start;

			textureChunk->set_datachunk(reinterpret_cast<const char*>(&textureData[start]), dataSize);
			textureChunk->set_chunknumber(chunkNumber);
			textureChunk->set_maxchunknumber(totalChunks - 1);
			textureChunk->set_islastchunk(chunkNumber == totalChunks - 1);
			textureChunk->set_width(width);
			textureChunk->set_height(height);
		}

		if (!writer->Write(textureTable))
		{
			return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to stream texture data.");
		}
	}

	return grpc::Status::OK;
}


grpc::Status LevelDataServer::GetMeshDataChunk(grpc::ServerContext* context, const MeshChunkRequest* request, MeshDataChunk* response)
{
    return grpc::Status::OK;
}

grpc::Status LevelDataServer::GetTextureDataChunk(grpc::ServerContext* context, const TextureChunkRequest* request, TextureDataChunk* response)
{
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
