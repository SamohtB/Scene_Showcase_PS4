#include "DownloadScreen.h"
#include "Texture.h"
#include "Debug.h"
#include "ClientRequestManager.h"
#include "GameObjectManager.h"

#include <imgui.h>
#include <sstream>

DownloadScreen::DownloadScreen() : AUIScreen("DownloadScreen")
{
    for (int i = 0; i < 5; ++i)
    {
        textureList.push_back(std::make_shared<Texture>("Models/Circuit.jpg", i, true));
    }
}

DownloadScreen::~DownloadScreen()
{
}

void DownloadScreen::drawUI()
{
    ImGui::Begin("Scene Picker", 0);

    ImVec2 imageSize(200, 200);

    for (size_t i = 0; i < textureList.size(); i++)
    {
        if (textureList[i] && textureList[i]->getTexture())
        {
            ImGui::BeginGroup();
            GLuint textureID = textureList[i]->getTexture();
            ImGui::Image(textureID, imageSize);

            ImGui::ProgressBar(0.0f, ImVec2(200, 10));
            
            std::string deleteID = "Delete##" + std::to_string(i);
            std::string reloadID = "Load##" + std::to_string(i);

            switch (this->isActive[i])
            {

            case true:

                if (ImGui::Button(deleteID.c_str(), ImVec2(200, 20)))
                {
                    this->isActive[i] = false;
                    //GameObjectManager::getInstance()->deleteScene(i);
                    Debug::Log("Scene " + std::to_string(i) + " deleted\n");
                }
                break;

            case false:

                if (ImGui::Button(reloadID.c_str(), ImVec2(200, 20)))
                {
                    this->isActive[i] = true;
                    Debug::Log("Requesting " + std::to_string(i) + "...\n");
                    ClientRequestManager::getInstance()->RequestScene(i);
                }
            }

            ImGui::EndGroup();

            if (i < textureList.size() - 1)
            {
                ImGui::SameLine();
            }
        }
    }

    ImGui::End();
  
}
