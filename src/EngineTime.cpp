#include "EngineTime.h"
#include <thread>

EngineTime* EngineTime::sharedInstance = nullptr;

EngineTime::EngineTime() {}

EngineTime::~EngineTime() {}

void EngineTime::initialize()
{
	sharedInstance = new EngineTime();
}

double EngineTime::getDeltaTime()
{
	return sharedInstance->deltaTime;
}

double EngineTime::getFixedDeltaTime()
{
	if (sharedInstance->targetFrameRate >= 0)
	{
		return sharedInstance->fixedDeltaTime;
	}

	return sharedInstance->deltaTime;
}

void EngineTime::setFrameTime(int time)
{
	sharedInstance->fixedDeltaTime = 1.0 / time;
	sharedInstance->targetFrameRate = time;
}

void EngineTime::LogFrameStart()
{
	sharedInstance->start = std::chrono::system_clock::now();
}

void EngineTime::LogFrameEnd()
{
	sharedInstance->end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = sharedInstance->end - sharedInstance->start;

	if (sharedInstance->targetFrameRate >= 0)
	{
		if (elapsed_seconds.count() < sharedInstance->fixedDeltaTime)
		{
			double sleepTime = sharedInstance->fixedDeltaTime - elapsed_seconds.count();
			std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
			sharedInstance->end = std::chrono::system_clock::now();
			elapsed_seconds = sharedInstance->end - sharedInstance->start;
		}
	}

	sharedInstance->deltaTime = elapsed_seconds.count();
}


