#pragma once

class LevelDataServer;

class ServerMain
{
public:
	ServerMain();
	~ServerMain();

	void run();

private:
	LevelDataServer* serverService = nullptr;
};

