#pragma once
#include "AUIScreen.h"

class MenuBar : public AUIScreen
{
public:

	MenuBar();
	~MenuBar();

	void drawUI() override;
	void updateFPS(double deltaTime);

private:
	double fps = 0.0;
};

