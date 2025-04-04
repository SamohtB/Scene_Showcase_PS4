#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>	

#include "AUIScreen.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UINames {
public:
	const String CONSOLE_SCREEN = "CONSOLE_SCREEN";
	const String MENU_BAR = "MENU_BAR";
	const String DOWNLOAD_SCREEN = "DOWNLOAD_SCREEN";
};


class UIManager
{
public: 
	typedef std::string String;
	typedef std::vector<AUIScreen*> UIList;
	typedef std::unordered_map<String, AUIScreen*> UITable;

	static UIManager* getInstance();
	static void initialize(GLFWwindow* gameWindow);
	static void destroy();

	void drawAllUI();
	void updateFPS(double deltaTime);

	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

private:
	UIManager(GLFWwindow* gameWindow);
	~UIManager();
	UIManager(UIManager const&) {};             // copy constructor is private
	UIManager& operator=(UIManager const&) {};  // assignment operator is private*/
	static UIManager* sharedInstance;

	UIList uiList;
	UITable uiTable;
};

