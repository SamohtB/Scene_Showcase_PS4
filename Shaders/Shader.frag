#version 330 core 

out vec4 FragColor; //pixel color

uniform sampler2D tex0;

in vec2 texCoord;
in vec3 fragPos;

void main()
{
	FragColor = texture(tex0, texCoord);
}