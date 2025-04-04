#include "UIManager.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "MenuBar.h"
#include "ConsoleScreen.h"
#include "Debug.h"
#include "DownloadScreen.h"

UIManager* UIManager::sharedInstance = nullptr;

UIManager* UIManager::getInstance()
{
    return sharedInstance;
}

void UIManager::initialize(GLFWwindow* gameWindow)
{
    sharedInstance = new UIManager(gameWindow);
}

void UIManager::destroy()
{
    delete sharedInstance;
}

void UIManager::drawAllUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (int i = 0; i < this->uiList.size(); i++) 
	{
		this->uiList[i]->drawUI();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}

void UIManager::updateFPS(double fps)
{
	UINames names;
	MenuBar* bar = (MenuBar*)this->uiTable[names.MENU_BAR];
	bar->updateFPS(fps);
}

UIManager::UIManager(GLFWwindow* gameWindow)
{
	const char* glsl_version = "#version 130";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(gameWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//populate UI table
	UINames uiNames;

	ConsoleScreen* consoleScreen = new ConsoleScreen();
	Debug::assignConsole(consoleScreen);
	this->uiTable[uiNames.CONSOLE_SCREEN] = consoleScreen;
	this->uiList.push_back(consoleScreen);

	MenuBar* menuBar = new MenuBar();
	this->uiTable[uiNames.MENU_BAR] = menuBar;
	this->uiList.push_back(menuBar);

	DownloadScreen* downloadScreen = new DownloadScreen();
	this->uiTable[uiNames.DOWNLOAD_SCREEN] = downloadScreen;
	this->uiList.push_back(downloadScreen);
}

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
