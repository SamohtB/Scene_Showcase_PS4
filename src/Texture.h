#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Texture
{
public:
	using String = std::string;
	Texture(String textureFilePath, int id);
	~Texture();

	GLuint getTexture();
	int getId();
	std::string getName();
	const GLuint& getTextureData();

private:
	GLuint texture;
	int id = 0;
	String name;
};