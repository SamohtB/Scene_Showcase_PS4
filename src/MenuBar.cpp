#include "MenuBar.h"
#include "Texture.h"
#include "Debug.h"
#include <imgui.h>
#include <sstream>

MenuBar::MenuBar() : AUIScreen("MenuBar")
{
}

MenuBar::~MenuBar()
{
}

void MenuBar::updateFPS(double fps)
{
    this->fps = fps;
}

void MenuBar::drawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        std::stringstream fpsText;
        fpsText << "FPS: " << static_cast<int>(fps);
        ImGui::Text("%s", fpsText.str().c_str());

        ImGui::EndMainMenuBar();
    }
}