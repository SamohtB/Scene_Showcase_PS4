#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Texture
{
public:
	using String = std::string;
	Texture(String textureFilePath);
	~Texture();

	GLuint getTexture();

protected:
	GLuint texture;
};