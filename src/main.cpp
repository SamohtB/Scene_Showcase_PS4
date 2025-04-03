#include "Game.h"
#include "ServerMain.h"

bool isServer = true;

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