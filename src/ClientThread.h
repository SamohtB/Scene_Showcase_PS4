#pragma once
#include <winsock2.h>

#include "IETThread.h"
class ClientThread :    public IETThread
{
public:
	ClientThread();
	~ClientThread() = default;

	void clientStart(); //calling this directly is a non-threading call.
	void sendMessage(std::string msg) const;

protected:
	void run() override;

private:
	SOCKET serverSocket = INVALID_SOCKET;
	int iResult = 9999;
};

