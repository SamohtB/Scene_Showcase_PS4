#include "ServerMain.h"

#include "LevelDataServer.h"

ServerMain::ServerMain()
{
	this->serverService = new LevelDataServer();

	/* Add Scenes and Objects */
}

ServerMain::~ServerMain()
{
	/* Remove Scenes */
}

void ServerMain::run()
{
	this->serverService->start();

	while (true)
	{
		IETThread::sleep(100);
	}
}
