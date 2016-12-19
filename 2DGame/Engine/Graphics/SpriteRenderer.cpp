//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpriteRenderer.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include "..\Base\GameCore.h"
#include "..\Base\ContentManager.h"

#include "..\Graphics\VertexHelper.h"


ShaderProgram* SpriteRenderer::m_pShaderProgram = nullptr;
GLuint SpriteRenderer::m_VBO = 0;
GLuint SpriteRenderer::m_VAO = 0;
bool SpriteRenderer::m_IsInitialized = false;
unsigned int SpriteRenderer::m_BufferSize = 0;
std::vector<SpriteVertex> SpriteRenderer::m_Sprites;


void SpriteRenderer::Initialize()
{
	m_pShaderProgram = ContentManager::LoadShader(L"./Resources/Shaders/Sprite.vert.glsl",
												  L"./Resources/Shaders/Sprite.frag.glsl",
												  L"./Resources/Shaders/Sprite.geom.glsl",
												  L"SPRITESHADER");
	InitRenderData();
	m_IsInitialized = true;
}

void SpriteRenderer::Release()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
	m_IsInitialized = false;

	m_Sprites.clear();
}

void SpriteRenderer::DrawSprite(unsigned int textureID, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec4 color, glm::vec2 pivot, bool worldspace)
{
	//Add sprite to the Sprites vector 
	SpriteVertex sprite;
	sprite.TextureID = textureID;
	sprite.Color = color;
	sprite.TransformData = glm::vec4(position, 0, rotate);
	sprite.TransformData2 = glm::vec4(pivot, size);
	sprite.IsInWorldspace = worldspace;

	m_Sprites.push_back(sprite);
}

void SpriteRenderer::Draw(const GameContext & gameContext)
{
	//if there are no sprite return
	if (m_Sprites.size() <= 0)
		return;

	//Update the sprite buffer
	UpdateBuffer();

	//use the sprite shader
	m_pShaderProgram->Use();

	unsigned int batchSize = 1;
	unsigned int  batchOffset = 0;
	unsigned int  spriteCount = m_Sprites.size();

	for (unsigned int  i = 0; i < spriteCount; ++i)
	{
		if (i < (spriteCount - 1) && m_Sprites[i].TextureID == m_Sprites[i + 1].TextureID &&  m_Sprites[i].IsInWorldspace == m_Sprites[i + 1].IsInWorldspace)
		{
			++batchSize;
			continue;
		}

		//Set projection matrix
		if(m_Sprites[i].IsInWorldspace)
			m_pShaderProgram->SetUniform("matProjection", gameContext.pCamera->GetProjection() *  gameContext.pCamera->GetCameraTransform());
		else
			m_pShaderProgram->SetUniform("matProjection", gameContext.pCamera->GetProjection());

		//Get Texture
		const Texture2D* texture = ContentManager::GetTexture( m_Sprites[i].TextureID );
		
		//Bind the texture
		GLuint textureID = texture->GetID();		

		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_POINTS,batchOffset, batchSize);

		batchOffset += batchSize;
		batchSize = 1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	//Stop the sprite shader progream
	m_pShaderProgram->StopUsing();

	m_Sprites.clear();
}

void SpriteRenderer::InitRenderData()
{

	m_pShaderProgram->Link();

}

void SpriteRenderer::UpdateBuffer()
{
	// if the vertex buffer does not exists, or the number of sprites is bigger then the buffer size
	if (m_VBO == 0 || m_BufferSize < sizeof(SpriteVertex) * m_Sprites.size() )
	{
		m_BufferSize = sizeof(SpriteVertex) * m_Sprites.size();
		// Create VBO with point coordinates
		glGenBuffers(1, &m_VBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * m_Sprites.size(), m_Sprites.data(), GL_STATIC_DRAW);

		// Create VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// Specify layout of point data
		GLuint transformDataAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vTransformData");
		GLuint transformData2Attrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vTransformData2");
		GLuint colorAttrib = glGetAttribLocation(m_pShaderProgram->GetID(), "vColor");

		glEnableVertexAttribArray(transformDataAttrib);
		glVertexAttribPointer(transformDataAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), 0);
		
		glEnableVertexAttribArray(transformData2Attrib);
		glVertexAttribPointer(transformData2Attrib, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),(GLvoid*)(sizeof(glm::vec4)) );

		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)(sizeof(glm::vec4) * 2));
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SpriteVertex) * m_Sprites.size(), m_Sprites.data());
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

