#pragma once
#include <memory>
#include "ThreadPool.h"

class ClientRequestManager
{
public:
    static ClientRequestManager* getInstance();
    static void initialize();
    static void destroy();

    void RequestScene(int sceneId);

private:
    ClientRequestManager();
    ~ClientRequestManager();
    ClientRequestManager(ClientRequestManager const&) {}
    ClientRequestManager& operator=(ClientRequestManager const&) {}

    static ClientRequestManager* sharedInstance;
    std::unique_ptr<ThreadPool> threadPool;
};

