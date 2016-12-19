//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Texture2D.h"

Texture2D::Texture2D()
{
	//Generate a texture ID
	glGenTextures(1, &m_ID);
}

Texture2D::~Texture2D()
{
	//Delete the texture ID
	glDeleteTextures(1, &m_ID);
}

void Texture2D::Generate(int width, int height, const unsigned char* data)
{
	m_Width  = width;
	m_Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, m_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_TextureSettings.Internal_Format, width, height, 0, m_TextureSettings.Image_Format, GL_UNSIGNED_BYTE, data);
	
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TextureSettings.Wrap_U);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TextureSettings.Wrap_V);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TextureSettings.Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TextureSettings.Filter_Max);
	
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}