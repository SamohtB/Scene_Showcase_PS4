#include "ClientRequestManager.h"
#include "LevelDataClient.h"
#include <grpcpp/create_channel.h>

ClientRequestManager* ClientRequestManager::sharedInstance = nullptr;

ClientRequestManager* ClientRequestManager::getInstance()
{
    return sharedInstance;
}

void ClientRequestManager::initialize()
{
    sharedInstance = new ClientRequestManager();
}

void ClientRequestManager::destroy()
{
    delete sharedInstance;
}

void ClientRequestManager::RequestScene(int sceneId)
{
    std::shared_ptr<grpc::ChannelInterface> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    LevelDataClient* clientRequest = new LevelDataClient(channel);
    clientRequest->setSceneIdRequest(sceneId);
    this->threadPool->scheduleTask(clientRequest);
}

ClientRequestManager::ClientRequestManager()
{
    this->threadPool = std::make_unique<ThreadPool>("RequestPool", 4);
    this->threadPool->startScheduler();
}

ClientRequestManager::~ClientRequestManager()
{
}
