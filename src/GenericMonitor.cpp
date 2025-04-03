#include "GenericMonitor.h"

GenericMonitor::GenericMonitor()
{

}

void GenericMonitor::tryEnter()
{
	std::unique_lock<std::mutex> lock(this->lock);

	while (!this->isComplete)
	{
		this->condition.wait(lock);
	}

	this->isComplete = false;
}

void GenericMonitor::reportExit()
{
	std::lock_guard<std::mutex> lock(this->lock);
	this->isComplete = true;
	this->condition.notify_one();
}

void GenericMonitor::notifyComplete()
{
	std::lock_guard<std::mutex> lock(this->lock);
	this->isComplete = true;
	this->condition.notify_all();
}

void GenericMonitor::open()
{
	std::lock_guard<std::mutex> lock(this->lock);
	this->isComplete = true;
}
