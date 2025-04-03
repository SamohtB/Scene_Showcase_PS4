#include "Game.h"
#include "ServerMain.h"

//#define ISSERVER 1

int main()
{
#ifdef ISSERVER

	Game game;
	game.run();

#else

	ServerMain server;
	server.run();

#endif
}