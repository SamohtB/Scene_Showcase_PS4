#include "UIManager.h"

#include "ConsoleScreen.h"
#include "Debug.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UIManager* UIManager::sharedInstance = nullptr;

UIManager* UIManager::getInstance()
{
    return sharedInstance;
}

void UIManager::initialize()
{
    sharedInstance = new UIManager();
}

void UIManager::destroy()
{
    delete sharedInstance;
}

void UIManager::drawAllUI()
{
	for (int i = 0; i < this->uiList.size(); i++) {
		this->uiList[i]->drawUI();
	}
}

UIManager::UIManager()
{
	//populate UI table
	UINames uiNames;
	ConsoleScreen* consoleScreen = new ConsoleScreen();
	Debug::assignConsole(consoleScreen);
	this->uiTable[uiNames.CONSOLE_SCREEN] = consoleScreen;
	this->uiList.push_back(consoleScreen);
}

UIManager::~UIManager()
{
}
