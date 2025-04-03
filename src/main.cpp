#include "Game.h"
#include "ServerMain.h"

bool isServer = false;

int main()
{
	if (isServer)
	{
		ServerMain server;
		server.run();
	}
	else
	{
		Game game;
		game.run();
	}
}