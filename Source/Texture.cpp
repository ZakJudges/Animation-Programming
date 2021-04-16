#include "Texture.h"
#include "glad.h"
#include "stb_image.h"
#include <iostream>
#include "Timer.h"

Texture::Texture()
{
    m_width = 0;
    m_height = 0;
    m_channels = 0;
    glGenTextures(1, &m_handle);
}

Texture::Texture(const char* path)
{
    glGenTextures(1, &m_handle);

    Load(path);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}

void Texture::Load(const char* path)
{
    glBindTexture(GL_TEXTURE_2D, m_handle);
    int width, height, channels;
    //  Load texture with 4 channels (RGBA).
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);

    //  Upload the texture to the GPU.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_width = width;
    m_height = height;
    m_channels = channels;
}

void Texture::Set(unsigned int uniform, unsigned int texIndex)
{
    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glUniform1i(uniform, texIndex);
}

void Texture::UnSet(unsigned int texIndex)
{
    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

unsigned int Texture::GetHandle()
{
    return m_handle;
}
