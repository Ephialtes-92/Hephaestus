#pragma once

#include <string>
#include <filesystem>

#include <ThirdParty/glad/glad.h>

class Texture
{
public:
	explicit Texture(std::string textureName);
	~Texture();

	Texture(const Texture &) = delete;
	Texture& operator=(const Texture&) = delete;

	//Move and copy constructors
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	unsigned int getId() { return m_ID; }



private:
	unsigned int m_ID = 0;

	int m_Width = 0;
	int m_Height = 0;
	int m_Channels = 0;

};