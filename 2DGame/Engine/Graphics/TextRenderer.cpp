//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TextRenderer.h"

#include "..\Graphics\Texture2D.h"
#include "..\Graphics\SpriteFont.h"
#include "..\Graphics\ShaderProgram.h"

#include "..\Base\GeneralStructs.h"
#include "..\Base\ContentManager.h"

ShaderProgram* TextRenderer::m_pShaderProgram = nullptr;
GLuint TextRenderer::m_VBO = 0;
GLuint TextRenderer::m_VAO = 0;
bool TextRenderer::m_IsInitialized = false;
unsigned int TextRenderer::m_BufferSize = 0;
std::vector<SpriteFont*> TextRenderer::m_SpriteFonts;
glm::mat4 TextRenderer::m_Transform;
unsigned int TextRenderer::m_NumCharacters = 0;
unsigned int TextRenderer::m_NumPrevCharacters = 0;


void TextRenderer::Initialize()
{
	m_pShaderProgram = ContentManager::LoadShader(L"./Resources/Shaders/Text.vert.glsl",
												  L"./Resources/Shaders/Text.frag.glsl",
												  L"./Resources/Shaders/Text.geom.glsl",
												  L"TEXTSHADER");
	m_pShaderProgram->Link();
	m_IsInitialized = true;
}

void TextRenderer::Release()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
	m_IsInitialized = false;

	for (size_t i = 0; i < m_SpriteFonts.size(); i++)
	{
		delete m_SpriteFonts[i];
	}

	m_SpriteFonts.clear();
}

void TextRenderer::Draw(const GameContext & gameContext)
{
	if (m_SpriteFonts.size() <= 0)
		return;

	UpdateBuffer();
	
	m_pShaderProgram->Use();
	m_pShaderProgram->SetUniform( "matProjection", gameContext.pCamera->GetProjection() );


	for(unsigned int  i = 0; i < m_SpriteFonts.size() ; ++i)
	{
		//Get Texture
		const Texture2D* texture = ContentManager::GetTexture( m_SpriteFonts[i]->m_TextureID );

		//Bind the texture
		GLuint textureID = texture->GetID();
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Set Texture Size
		glm::vec2 texSize = glm::vec2(texture->GetWidth(),texture->GetHeight()) ;
		m_pShaderProgram->SetUniform("uniTextureSize", texSize);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_POINTS, m_SpriteFonts[i]->m_BufferStart, m_SpriteFonts[i]->m_BufferSize);

		m_SpriteFonts[i]->m_IsAddedToRenderer = false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	m_pShaderProgram->StopUsing();

	m_SpriteFonts.clear();
}

void TextRenderer::DrawText(SpriteFont * pFont, const std::wstring & text, glm::vec2 position, glm::vec4 color)
{
	m_NumCharacters += wcslen(text.c_str());
	pFont->m_TextCache.push_back(TextCache(text, position, color));
	if (!pFont->m_IsAddedToRenderer)
	{
		m_SpriteFonts.push_back(pFont);
		pFont->m_IsAddedToRenderer = true;
	}
}

void TextRenderer::UpdateBuffer()
{
	if (m_VBO == 0 || m_NumCharacters > m_BufferSize)
	{
		//Release the buffer if it exists
		glDeleteBuffers(1, &m_VBO);
		glDeleteVertexArrays(1, &m_VAO);

		//Set buffersize if needed
		if (m_NumCharacters > m_BufferSize)
			m_BufferSize = m_NumCharacters;


		// Create VBO with point coordinates
		glGenBuffers(1, &m_VBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TextVertex) * m_BufferSize,nullptr, GL_STATIC_DRAW);

		// Create VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Specify layout of point data
		
		GLuint channelAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vChannel");
		GLuint positionAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vPosition");
		GLuint colorAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vColor");
		GLuint texCoordAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vTexCoord");
		GLuint charSizeAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vCharSize");

		unsigned int ptr = 0;
		glEnableVertexAttribArray(channelAttrib);
		glVertexAttribPointer(channelAttrib, 1, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)(ptr) );
		
		ptr += sizeof(float);
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)(ptr) );
		
		ptr += sizeof(glm::vec3);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)(ptr) );

		ptr += sizeof(glm::vec4);
		glEnableVertexAttribArray(texCoordAttrib);
		glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)(ptr));

		ptr += sizeof(glm::vec2);
		glEnableVertexAttribArray(charSizeAttrib);
		glVertexAttribPointer(charSizeAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (GLvoid*)(ptr));
	}

	//Fill Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);



	if (m_NumPrevCharacters > m_NumCharacters)
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextVertex) * m_BufferSize, nullptr);

	m_NumPrevCharacters = m_NumCharacters;
	
	int bufferPosition = 0;
	for each (SpriteFont* pFont in m_SpriteFonts)
	{
		pFont->m_BufferSize = 0;
		pFont->m_BufferStart = bufferPosition / sizeof(TextVertex);
		for each(TextCache cache in pFont->m_TextCache)
		{
			CreateTextVertices(pFont, cache ,bufferPosition);

		}
		pFont->m_BufferSize = bufferPosition / sizeof(TextVertex) - pFont->m_BufferStart;
		pFont->m_TextCache.clear(); //Clear the cache
	}
	
	m_NumCharacters = 0;
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::CreateTextVertices(SpriteFont * pFont, const TextCache & info, int & bufferPosition)
{
	int totalAdvanceX = 0;
	for (wchar_t charId : info.Text)
	{
		if (SpriteFont::IsCharValid(charId) && pFont->GetMetric(charId).IsValid)
		{
			auto metric = pFont->GetMetric(charId);

			if (charId == ' ')
			{
				totalAdvanceX += metric.AdvanceX;
				continue;
			}

			TextVertex vertexText;
			vertexText.Position.x = info.Position.x + totalAdvanceX + metric.OffsetX;
			vertexText.Position.y = info.Position.y - metric.Height - metric.OffsetY;
			vertexText.Position.z = 0.9f;
			vertexText.Color = info.Color;
			vertexText.TexCoord = metric.TexCoord;
			vertexText.CharacterDimension = glm::vec2(metric.Width, metric.Height);
			vertexText.ChannelId = metric.Channel;

			glBufferSubData(GL_ARRAY_BUFFER, bufferPosition, sizeof(TextVertex), &vertexText);
			bufferPosition += sizeof(TextVertex);
			totalAdvanceX += metric.AdvanceX;

		}
		else
		{
			Logger::Log(LogLevel::Warning, L"TextRenderer::CreateTextVertices > Char not supported for current font.\nCHARACTER: " + std::to_wstring((int)charId) + L" FONT: " + pFont->m_FontName);
		}

	}
}
