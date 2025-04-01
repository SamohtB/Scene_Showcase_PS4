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
}

UIManager::UIManager(GLFWwindow* gameWindow)
{
	const char* glsl_version = "#version 130";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(gameWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//populate UI table
	UINames uiNames;
	ConsoleScreen* consoleScreen = new ConsoleScreen();
	Debug::assignConsole(consoleScreen);
	this->uiTable[uiNames.CONSOLE_SCREEN] = consoleScreen;
	this->uiList.push_back(consoleScreen);
}

UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
