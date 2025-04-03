#pragma once
#include <condition_variable>

class GenericMonitor
{
public:
    GenericMonitor();
    
    virtual void tryEnter();
    virtual void reportExit();
    virtual void notifyComplete();
    void open();

protected:
    std::mutex lock;
    std::condition_variable condition;
    bool isComplete = false;
};

