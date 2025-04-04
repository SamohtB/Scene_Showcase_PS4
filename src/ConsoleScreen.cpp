#include "ConsoleScreen.h"
#include <sstream>
#include "EngineTime.h" 

void ConsoleScreen::appendText(String text)
{
	std::stringstream buffer;
	buffer << this->lineCount << " " << text;

	this->textLog->appendf(buffer.str().c_str());
	this->lineCount++;
}

ConsoleScreen::ConsoleScreen(): AUIScreen("ConsoleScreen")
{
	this->textLog = new ImGuiTextBuffer();
}

ConsoleScreen::~ConsoleScreen()
{
	delete this->textLog;
}

void ConsoleScreen::drawUI()
{
	ImGui::Begin("Console", 0);
	if (ImGui::Button("Clear")) { this->textLog->clear(); this->lineCount = 0; }
	ImGui::TextUnformatted(this->textLog->begin(), this->textLog->end());
	ImGui::End();
}
