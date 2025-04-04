#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Transform.h"

class LevelDataServer;
class RawMesh;
class Texture;

class ServerMain
{
public:
	struct ObjectData
	{
		std::string name;
		Transform transform;
		int meshId;
		int textureId;
	};

	ServerMain();
	~ServerMain();

	void run();

private:
	LevelDataServer* serverService = nullptr;

	std::unordered_map<int, std::vector<ObjectData>> objectHolder;
	std::unordered_map<int, std::vector<std::shared_ptr<RawMesh>>> meshHolder;
	std::unordered_map<int, std::vector<std::shared_ptr<Texture>>> textureHolder;
};

