#include "ServerThread.h"

#include <format>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Debug.h"
#include "EventBroadcaster.h"
#include "EventNames.h"
#include "NetworkManager.h"
#pragma comment(lib, "Ws2_32.lib")

ServerThread::ServerThread()
{
}

void ServerThread::serverStart()
{
	struct addrinfo* result = nullptr, * ptr = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	this->iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
	if (this->iResult != 0) {
		Debug::Log("Failed to initialize network config. Please try again. \n");
		WSACleanup();
		return;
	}
	SOCKET listenSocket = NULL;
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET)
	{
		Debug::Log("Failed to initialize a socket. \n");
		WSACleanup();
	}

	NetworkManager::getInstance()->serverState = NetworkManager::ServerState::WAITING_CONNECTION;
	//bind socket
	this->iResult = bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	freeaddrinfo(result);

	if (this->iResult == SOCKET_ERROR)
	{
		Debug::Log("Failed to initialize a socket. \n");
		closesocket(listenSocket);
		WSACleanup();
	}
	else
	{
		Debug::Log("Successfully bound socket! \n");
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		Debug::Log("Listening to a socket failed. \n");
		closesocket(listenSocket);
		WSACleanup();
	}

	//attempt to accept any incoming client connection. Note that accept is a blocking call!
	Debug::Log("Looking for incoming connections. \n");

	this->clientSocket = accept(listenSocket, nullptr, nullptr);
	if (this->clientSocket == INVALID_SOCKET)
	{
		Debug::Log("No incoming sockets accepted. \n");
		closesocket(listenSocket);
		WSACleanup();
	}

	//no longer needs a listening socket since only one client is allowed
	closesocket(listenSocket);

	//Receive messages from client until it either both of them shuts down.
	NetworkManager::getInstance()->serverState = NetworkManager::ServerState::CLIENTS_CONNECTED;

	int iSendResult;
	char recvbuf[DEFAULT_BUFFER_LEN];
	int recvbuflen = DEFAULT_BUFFER_LEN;
	// Receive until the peer shuts down the connection
	do {
		this->iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (this->iResult > 0) {
			std::shared_ptr<Parameters> params = std::make_shared<Parameters>(EventNames::ON_RECEIVED_MSG);
			params->encodeString(ParameterKeys::MSG_KEY, recvbuf);
			params->encodeInt(ParameterKeys::SOURCE_KEY, 1);

			EventBroadcaster::getInstance()->broadcastEventWithParams(EventNames::ON_RECEIVED_MSG, params);
			printf("[Server] Bytes received: %d\n", this->iResult);

			// // Echo the buffer back to the sender
			// iSendResult = send(this->clientSocket, recvbuf, this->iResult, 0);
			// if (iSendResult == SOCKET_ERROR) {
			// 	printf("send failed with error: %d\n", WSAGetLastError());
			// 	closesocket(this->clientSocket);
			// 	WSACleanup();
			// 	return;
			// }
			//
			// printf("[Server] Bytes sent: %d\n", iSendResult);
		}
		else if (this->iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(this->clientSocket);
			WSACleanup();
			return;
		}

		// std::cout << "iResult: " << this->iResult << "\n" << std::endl;
	} while (iResult > 0);

}

void ServerThread::sendMessage(std::string msg)
{
	this->hasPendingMsg = true;
	this->msgToSend = msg;
	int iSendResult = 0;

	if (this->hasPendingMsg)
	{
		iSendResult = send(this->clientSocket, this->msgToSend.c_str(), this->iResult, 0);
		std::string debugMsg = std::format("Message sent to client: {} \n", this->msgToSend);
		std::cout << debugMsg << std::endl;

		if (iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(this->clientSocket);
			WSACleanup();
			return;
		}

		Debug::Log(debugMsg);
		this->hasPendingMsg = false;
		this->msgToSend = "";
	}
	else
	{
		std::cout << "Has no pending msg. iResult: " << this->iResult << std::endl;
	}
}

void ServerThread::run()
{
	this->serverStart();
}
