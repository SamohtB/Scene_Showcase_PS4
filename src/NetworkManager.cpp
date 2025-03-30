#include "NetworkManager.h"
#include "imgui_impl_opengl3_loader.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Debug.h"
#include "ServerThread.h"

NetworkManager* NetworkManager::sharedInstance = nullptr;

NetworkManager* NetworkManager::getInstance()
{
	return sharedInstance;
}

void NetworkManager::initialize()
{
	sharedInstance = new NetworkManager();
}

void NetworkManager::destroy()
{
	WSACleanup();
	delete sharedInstance;
}

NetworkManager::NetworkManager()
{
	WSADATA wsaData;

	int result;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result == 0)
	{
		Debug::Log("Successfully initialized winsock \n");
		this->serverThread = std::make_shared<ServerThread>();
		this->clientThread = std::make_shared<ClientThread>();
	}
	else
	{
		Debug::Log("Winsock failed. \n");
	}
}

void NetworkManager::serverStart()
{
	this->entityType = EntityType::SERVER;
	if(this->threadingEnabled)
	{
		this->serverThread->start();
	}
	else
	{
		this->serverThread->serverStart();
	}
}

void NetworkManager::clientStart()
{
	this->entityType = EntityType::CLIENT;
	if (this->threadingEnabled)
	{
		this->clientThread->start();
	}
	else
	{
		this->clientThread->clientStart();
	}
}

NetworkManager::ServerState NetworkManager::getServerState() const
{
	return this->serverState;
}

NetworkManager::ClientState NetworkManager::getClientState() const
{
	return this->clientState;
}

NetworkManager::EntityType NetworkManager::getEntityType() const
{
	return this->entityType;
}

void NetworkManager::setThreadingEnabled(bool flag)
{
	this->threadingEnabled = flag;
}

void NetworkManager::sendMessageAsServer(std::string msg) const
{
	this->serverThread->sendMessage(msg);
}

void NetworkManager::sendMessageAsClient(std::string msg) const
{
	this->clientThread->sendMessage(msg);
}


