#include <renderer/textures/texture.h>
#include <ThirdParty/stb/stb_image.h>
#include <stdexcept>
#include <config.h>

std::filesystem::path textureDirectory = TEXTURES_DIR;

Texture::Texture(std::string textureName)
{
    stbi_set_flip_vertically_on_load(true);

    //Load the image
    std::filesystem::path texturePath = textureDirectory / textureName;
    unsigned char* data = stbi_load(texturePath.string().c_str(), &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);
    if (!data)
    {
        throw std::runtime_error(
            "Failed to load texture: " + texturePath.string());
    }

    //Generate the texture
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Teture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    //Leave OpenGL in a clean state
    glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

//Take over ownership of the other texture basically
Texture::Texture(Texture&& other) noexcept
    : m_ID(other.m_ID),
    m_Width(other.m_Width),
    m_Height(other.m_Height),
    m_Channels(other.m_Channels)
{
    other.m_ID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        glDeleteTextures(1, &m_ID);

        m_ID = other.m_ID;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_Channels = other.m_Channels;

        other.m_ID = 0;
    }

    return *this;
}