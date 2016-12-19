#pragma once
struct TextureSettings
{
	TextureSettings():
		Internal_Format(GL_RGB),
		Image_Format(GL_RGB),

		Wrap_U(GL_CLAMP),
		Wrap_V(GL_CLAMP),

		Filter_Min(GL_LINEAR),
		Filter_Max(GL_LINEAR)
	{}

	GLuint Internal_Format = GL_RGB;
	GLuint Image_Format = GL_RGB;

	GLuint Wrap_U = GL_CLAMP;
	GLuint Wrap_V = GL_CLAMP;

	GLuint Filter_Min = GL_LINEAR;
	GLuint Filter_Max = GL_LINEAR;
};

class Texture2D
{
public:

	Texture2D();
	~Texture2D();
	void Generate(int width, int height, const unsigned char* data);
	void SetTextureSettings(const TextureSettings & settings) { m_TextureSettings = settings; }
	void Bind() const;
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	GLuint GetID() const { return m_ID; }

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	//ID
	GLuint m_ID;

	//Dimensions
	int m_Width = 0;
	int	m_Height = 0;  

	//Settings
	TextureSettings m_TextureSettings;
};

