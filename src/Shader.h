#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class Shader
{
public:
	using String = std::string;
	Shader(String vertexShaderPath, String fragmentShaderPath);
	GLuint GetShaderProgram();

protected:
	GLuint shaderProgram;
};