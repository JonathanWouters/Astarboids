#pragma once
#include "..\Base\GeneralStructs.h"
class SpriteFont;
class ShaderProgram;
struct TextCache;

struct TextVertex
{
	unsigned int ChannelId;
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	glm::vec2 CharacterDimension;
};

class TextRenderer
{
public:
	static void Initialize();
	static void Release();
	static void Draw(const GameContext& gameContext);	
	static void DrawText(SpriteFont* pFont, const std::wstring& text, glm::vec2 position, glm::vec4 color = glm::vec4(1,1,1,1) );


private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	TextRenderer() {};
	~TextRenderer() {};

	static void UpdateBuffer();
	static void CreateTextVertices(SpriteFont* pFont, const TextCache& info, int& bufferPosition);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	static glm::mat4 m_Transform;
	
	static ShaderProgram* m_pShaderProgram;
	
	static GLuint m_VBO;
	static GLuint m_VAO;

	static unsigned int m_BufferSize;
	static unsigned int m_NumCharacters, m_NumPrevCharacters;

	static std::vector<SpriteFont*> m_SpriteFonts;
	static bool m_IsInitialized;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TextRenderer(const TextRenderer& t);
	TextRenderer& operator=(const TextRenderer& t);
};
