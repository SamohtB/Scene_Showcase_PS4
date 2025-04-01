#pragma once
#include "ServerThread.h"
#include "ClientThread.h"

#define DEFAULT_IP_ADDRESS "192.168.84.55" //TODO: Put your IP address here
#define DEFAULT_PORT "8088"
#define DEFAULT_BUFFER_LEN 512

class NetworkManager
{
public:
	static NetworkManager* getInstance();
	static void initialize();
	static void destroy();

	void serverStart();
	void clientStart();

	enum ServerState
	{
		SERVER_INACTIVE,
		WAITING_CONNECTION,
		CLIENTS_CONNECTED,
		CLOSED_CONNECTION
	};

	enum ClientState
	{
		CLIENT_INACTIVE,
		ATTEMPTING_SERVER_CONNECT,
		CONNECTED_TO_SERVER,
		CLOSED
	};

	enum EntityType
	{
		SERVER,
		CLIENT
	};

	ServerState getServerState() const;
	ClientState getClientState() const;
	EntityType getEntityType() const;

	void setThreadingEnabled(bool flag);
	void sendMessageAsServer(std::string msg) const;
	void sendMessageAsClient(std::string msg) const;

private:
	NetworkManager();
	~NetworkManager() = default;

	NetworkManager(NetworkManager const&) {};             // copy constructor is private
	NetworkManager& operator=(NetworkManager const&) {};  // assignment operator is private*/
	static NetworkManager* sharedInstance;

	ServerState serverState = ServerState::SERVER_INACTIVE;
	ClientState clientState = ClientState::CLIENT_INACTIVE;
	EntityType entityType = EntityType::SERVER;
	bool threadingEnabled = true;

	std::shared_ptr<ServerThread> serverThread = nullptr;
	std::shared_ptr<ClientThread> clientThread = nullptr;

	friend class ServerThread;
	friend class ClientThread;

};

