#include "ClientThread.h"
#include <format>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "ChatScreen.h"
#include "Debug.h"
#include "EventBroadcaster.h"
#include "NetworkManager.h"
// #pragma comment(lib, "Ws2_32.lib")

ClientThread::ClientThread()
{
}

void ClientThread::clientStart()
{
	NetworkManager::getInstance()->clientState == NetworkManager::ClientState::ATTEMPTING_SERVER_CONNECT;
	struct addrinfo* result = nullptr, * ptr = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	this->iResult = getaddrinfo(DEFAULT_IP_ADDRESS, DEFAULT_PORT, &hints, &result);

	for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		this->serverSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (this->serverSocket == INVALID_SOCKET) {
			wchar_t* s = nullptr;
			std::string errorMsg = std::format("Socket failed with error: {} \n", FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)s, sizeof(s), nullptr));
			printf("socket failed with error: %ld\n", WSAGetLastError());

			Debug::Log(errorMsg);
			WSACleanup();
		}

		// Connect to server.
		iResult = connect(this->serverSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(this->serverSocket);
			this->serverSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (this->serverSocket == INVALID_SOCKET)
	{
		Debug::Log("Unable to connect to server \n");
		WSACleanup();
		NetworkManager::getInstance()->clientState == NetworkManager::ClientState::CLIENT_INACTIVE;
		return;
	}

	//first initial send
	const char* sendMsg = "Client has successfully connected.";
	iResult = send(this->serverSocket, sendMsg, (int)strlen(sendMsg), 0);
	if (iResult == SOCKET_ERROR) {
		std::string errorMsg = std::format("Send failed with error: {} \n", WSAGetLastError());
		printf("send failed: %d\n", WSAGetLastError());

		Debug::Log(errorMsg);
		closesocket(this->serverSocket);
		WSACleanup();
		NetworkManager::getInstance()->clientState = NetworkManager::ClientState::CLIENT_INACTIVE;
		return;
	}

	printf("[Client] Bytes Sent: %ld\n", iResult);
	NetworkManager::getInstance()->clientState = NetworkManager::ClientState::CONNECTED_TO_SERVER;

	// // shutdown the connection since no more data will be sent
	// iResult = shutdown(connectSocket, SD_SEND);
	// if (iResult == SOCKET_ERROR) {
	// 	printf("shutdown failed with error: %d\n", WSAGetLastError());
	// 	closesocket(connectSocket);
	// 	WSACleanup();
	// 	return;
	// }

	char recvbuf[DEFAULT_BUFFER_LEN];
	int recvbuflen = DEFAULT_BUFFER_LEN;
	// Receive until the peer closes the connection
	do {

		iResult = recv(this->serverSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			std::shared_ptr<Parameters> params = std::make_shared<Parameters>(EventNames::ON_RECEIVED_MSG);
			params->encodeString(ParameterKeys::MSG_KEY, recvbuf);
			params->encodeInt(ParameterKeys::SOURCE_KEY, 0);

			EventBroadcaster::getInstance()->broadcastEventWithParams(EventNames::ON_RECEIVED_MSG, params);
			printf("[Client] Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(this->serverSocket);
	WSACleanup();
	NetworkManager::getInstance()->clientState = NetworkManager::ClientState::CLIENT_INACTIVE;
}

void ClientThread::sendMessage(std::string msg) const
{
	std::vector<float> color;
	int iSendResult = send(this->serverSocket, msg.c_str(), this->iResult, 0);
	
	std::string debugMsg = std::format("Message sent to server: {} \n", msg);
	std::cout << debugMsg << std::endl;

	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(this->serverSocket);
		WSACleanup();
		return;
	}

	Debug::Log(debugMsg);
}

void ClientThread::run()
{
	this->clientStart();
}
