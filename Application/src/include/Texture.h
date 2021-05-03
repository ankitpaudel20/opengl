#pragma once

#include <string>
#include "stb_image/stb_image.h"

#include "core.h"

class Texture {
private:
	unsigned m_TextureID;
	std::string m_filepath;

	int m_width, m_height, m_BPP;

	void LoadFromFile(const std::string& path) {
		unsigned char* m_LocalBuffer;

		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLcall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_set_flip_vertically_on_load(1);

		m_filepath = path;
		m_LocalBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);
		if (!m_LocalBuffer) {
			std::cout << "texture file unable to load" << std::endl;
			assert(false);
		}

		GLcall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLcall(glGenerateMipmap(GL_TEXTURE_2D));
		this->Unbind();
		stbi_image_free(m_LocalBuffer);
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));
	}

public:
	std::string type;

	uint32_t getID() const { return m_TextureID; }

	std::string getPath() { return m_filepath; }

	/*Texture()
	{
		m_filepath = "\0";
		GLcall(glGenTextures(1, &m_TextureID));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	}*/

	Texture(const std::string& path) {
		m_filepath = "\0";
		GLcall(glGenTextures(1, &m_TextureID));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

		LoadFromFile(path);
	}

	//Texture(const Texture& in) = delete;

	Texture(const Texture& in) : m_TextureID(in.m_TextureID), m_filepath(in.m_filepath), m_width(in.m_width), m_height(in.m_height), m_BPP(in.m_BPP),type(in.type) {}

	Texture& operator=(const Texture& in) {
		m_TextureID = in.m_TextureID;
		m_filepath = in.m_filepath;
		m_width = in.m_width;
		m_height = in.m_height;
		m_BPP = in.m_BPP;
		return *this;
	}

	Texture(Texture&& in) noexcept : m_TextureID(in.m_TextureID), m_width(in.m_width), m_height(in.m_height), m_BPP(in.m_BPP), type(in.type) {
		m_filepath = std::move(in.m_filepath);
		in.m_TextureID = 0;
	}

	void free() {
		if (m_TextureID)
		{
			std::cout << "Texture freed\n";
		}
		GLcall(glDeleteTextures(1, &m_TextureID));
		m_TextureID = 0;
	}

	/*~Texture() {
		if (del) {
			std::cout << "Texture freed\n";
			GLcall(glDeleteTextures(1, &m_TextureID));
			m_TextureID = 0;
		}
	}*/

	void Bind(unsigned slot) const {
		GLcall(glActiveTexture(GL_TEXTURE0 + slot));
		GLcall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	}

	static void Unbind(unsigned slot = 0) {
		GLcall(glActiveTexture(GL_TEXTURE0 + slot));
		GLcall(glBindTexture(GL_TEXTURE_2D, 0));
	}
};
