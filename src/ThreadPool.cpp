#include "ThreadPool.h"
#include "PoolWorkerThread.h"
#include "ResourceMonitor.h"
#include "Debug.h"

ThreadPool::ThreadPool(String name, int worker_count) : name(name), workerCount(worker_count)
{
	for (int i = 0; i < workerCount; i++)
	{
		this->inactiveThreadList.push(new PoolWorkerThread(i, this));
	}

	this->workerCountMonitor = new ResourceMonitor(workerCount);
	this->taskCountMonitor = new ResourceMonitor(0);
}

ThreadPool::~ThreadPool()
{
	this->stopScheduler();

	for (auto& thread : activeThreadList)
	{
		delete thread.second;
	}

	this->activeThreadList.clear();

	while (!this->inactiveThreadList.empty())
	{
		delete this->inactiveThreadList.front();
		this->inactiveThreadList.pop();
	}

	delete this->workerCountMonitor;
}

void ThreadPool::startScheduler()
{
	this->isRunning = true;
	this->start();
}

void ThreadPool::stopScheduler()	
{
	this->isRunning = false;
}

void ThreadPool::scheduleTask(IWorkerAction* action)
{
	this->pendingActions.push(action);
	this->taskCountMonitor->reportExit(); //increase tasks by 1
}

void ThreadPool::run()
{
	while (this->isRunning)
	{
		this->taskCountMonitor->tryEnter();
		this->workerCountMonitor->tryEnter();

		PoolWorkerThread* worker_thread = this->inactiveThreadList.front();
		this->inactiveThreadList.pop();
		this->activeThreadList[worker_thread->getThreadID()] = worker_thread;

		worker_thread->assignTask(this->pendingActions.front());
		worker_thread->start();
		this->pendingActions.pop();
	}
}

void ThreadPool::onFinishedTask(int threadID)
{
	std::lock_guard<std::mutex> lock(this->workerMutex);

	if (this->activeThreadList[threadID] != nullptr)
	{
		auto worker = this->activeThreadList[threadID];
		this->activeThreadList.erase(threadID);
		this->inactiveThreadList.push(worker);
	}

	this->workerCountMonitor->reportExit();
}
