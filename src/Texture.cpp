#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

#include <filesystem>
#include <iostream>

Texture::Texture(String textureFilePath, int id, bool isRendered = true) : id(id), isRendered(isRendered)
{
    name = std::filesystem::path(textureFilePath).stem().string();

    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, colorChannels;

    const char* path = textureFilePath.c_str();
    unsigned char* tex_bytes = stbi_load(path, &img_width, &img_height, &colorChannels, 0);

    if (tex_bytes == nullptr) {
        std::cerr << "Failed to load texture file: " << textureFilePath << std::endl;
        return;
    }

    this->textureData.assign(tex_bytes, tex_bytes + img_width * img_height * colorChannels);
    this->width = img_width;
    this->height = img_height;
    this->colorChannels = colorChannels;

    stbi_image_free(tex_bytes);

    if (!isRendered) return;

    glGenTextures(1, &this->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    std::string strFileType = textureFilePath.substr(textureFilePath.length() - 3);

    if (strFileType == "png")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    }
    else if (strFileType == "jpg")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    }
    else if (strFileType == "tga")
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(std::vector<unsigned char> textureData, int id, bool isRendered, int width, int height, int channels)
{
    this->width = width;
    this->height = height;
    this->colorChannels = colorChannels;

    glGenTextures(1, &this->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    if (channels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture()
{
    glDeleteTextures(1, &this->texture);
}

GLuint Texture::getTexture()
{
    return this->texture;
}

int Texture::getId()
{
    return this->id;
}

std::string Texture::getName()
{
    return this->name;
}

const std::vector<unsigned char>& Texture::getTextureData()
{
    return this->textureData;
}

int Texture::getWidth()
{
    return this->width;
}

int Texture::getHeight()
{
    return this->height;
}

int Texture::getcolorChannels()
{
    return this->colorChannels;
}

int Texture::getSize()
{
    return this->width * this->height * this->colorChannels;
}
