#pragma once
#include "AUIScreen.h"
#include "imgui.h"
class ConsoleScreen :    public AUIScreen
{
public:
	void appendText(String text);
private:
	ConsoleScreen();
	~ConsoleScreen();

	virtual void drawUI() override;
	friend class UIManager;

	ImGuiTextBuffer* textLog;
	int lineCount = 0;
};

