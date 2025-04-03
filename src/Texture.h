#pragma once
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class Texture
{
public:
	using String = std::string;
	Texture(String textureFilePath, int id, bool isRendered);
	Texture(std::vector<unsigned char> textureData, int id, bool isRendered, int width, int height, int channels);
	~Texture();

	GLuint getTexture();
	int getId();
	std::string getName();
	const std::vector<unsigned char>& getTextureData();
	int getWidth();
	int getHeight();
	int getcolorChannels();
	int getSize();

private:
	GLuint texture;
	int id = 0;
	String name;
	bool isRendered = true;
	std::vector<unsigned char> textureData;
	int width = 0;
	int height = 0;
	int colorChannels = 0;
};