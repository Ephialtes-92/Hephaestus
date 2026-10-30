#pragma once

#include <string>

namespace hf::render
{

    enum class TextureType
    {
        Diffuse = 0,
        Specular
    };

    class Texture
    {
    public:
        explicit Texture(std::string textureName, std::string textureDirecory = "", TextureType type = TextureType::Diffuse);
        ~Texture();

        Texture(const Texture &) = delete;
        Texture& operator=(const Texture&) = delete;

        //Move and copy constructors
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        void bind(unsigned int slot = 0) const;
        void unbind() const;

        unsigned int getId() { return m_ID; }
        TextureType getType() { return m_Type; }



    private:
        unsigned int m_ID = 0;
        TextureType m_Type = TextureType::Diffuse;
		//Textures should have unique name within a model, so we can use the name to identify the texture
        std::string m_Name;

        int m_Width = 0;
        int m_Height = 0;
        int m_Channels = 0;

    };
}
