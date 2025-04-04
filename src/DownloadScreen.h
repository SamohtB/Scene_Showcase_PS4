#pragma once
#include "AUIScreen.h"
#include <memory>
#include <vector>

class Texture;
class DownloadScreen :  public AUIScreen
{
public:
	using TexturePtr = std::shared_ptr<Texture>;
	DownloadScreen();
	~DownloadScreen();

	// Inherited via AUIScreen
	void drawUI() override;

private:

	std::vector<TexturePtr> textureList;
	bool isActive[5] = { false, false, false, false, false };
};

