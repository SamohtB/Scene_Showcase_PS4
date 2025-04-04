#pragma once
#include <string>

class EventNames
{
public:
	typedef std::string String;
	inline static const String ON_SCENE_LOADED = "ON_SCENE_LOADED";
	inline static const String ON_RECEIVED_MSG = "ON_RECEIVED_MSG";
};

class ParameterKeys
{
public:
	typedef std::string String;
	inline static const String MSG_KEY = "MSG_KEY";
	inline static const String SOURCE_KEY = "SOURCE_KEY";
};

