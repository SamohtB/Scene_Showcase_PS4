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
	~Texture();

	GLuint getTexture();
	int getId();
	std::string getName();
	const std::vector<unsigned char>& getTextureData();

private:
	GLuint texture;
	int id = 0;
	String name;
	bool isRendered = true;
	std::vector<unsigned char> textureData;
};