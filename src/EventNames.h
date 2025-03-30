#pragma once
#include <string>

class EventNames
{
public:
	typedef std::string String;
	inline static const String ON_SCENE_LOADED = "ON_SCENE_LOADED";
	inline static const String ON_RECEIVED_MSG = "ON_RECEIVED_MSG";
	inline static const String ON_CHANGE_COLOR = "ON_CHANGE_COLOR";
};

class ParameterKeys
{
public:
	typedef std::string String;
	inline static const String MSG_KEY = "MSG_KEY";
	inline static const String SOURCE_KEY = "SOURCE_KEY";
	inline static const String COLOR_KEY = "COLOR_KEY";
};

