#include "ThreadPoolMonitor.h"
#include "IETThread.h"
ThreadPoolMonitor::ThreadPoolMonitor(int workerCount) : readyCount(workerCount) {}

void ThreadPoolMonitor::tryEnter()
{
    std::unique_lock<std::mutex> lock(this->lock);

    while (readyCount == 0) 
    {
        this->condition.wait(lock);
    }

    if (readyCount > 0) 
    {
        --readyCount;
    }
}

void ThreadPoolMonitor::reportExit()
{
    std::lock_guard<std::mutex> lock(this->lock);
    readyCount++;
    this->condition.notify_one();
}
