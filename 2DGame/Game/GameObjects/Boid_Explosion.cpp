//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Boid_Explosion.h"

#include "Engine\Base\ContentManager.h"
#include "Engine\Graphics\SpriteRenderer.h"

#define FRAME_00_IMG_LOCATION L"./Resources/Textures/Boid_Explosion_00.png"
#define FRAME_01_IMG_LOCATION L"./Resources/Textures/Boid_Explosion_01.png"
#define FRAME_02_IMG_LOCATION L"./Resources/Textures/Boid_Explosion_02.png"
#define FRAME_03_IMG_LOCATION L"./Resources/Textures/Boid_Explosion_03.png"

Boid_Explosion::Boid_Explosion(const glm::vec2 position, float angle,const glm::vec3 & color):
	m_Angle(angle),
	m_Color(color)
{
	m_Position = position;
}

Boid_Explosion::~Boid_Explosion()
{
}

void Boid_Explosion::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_Frame00_TextureID = ContentManager::LoadTexture(FRAME_00_IMG_LOCATION, true);
	m_Frame01_TextureID = ContentManager::LoadTexture(FRAME_01_IMG_LOCATION, true);
	m_Frame02_TextureID = ContentManager::LoadTexture(FRAME_02_IMG_LOCATION, true);
	m_Frame03_TextureID = ContentManager::LoadTexture(FRAME_03_IMG_LOCATION, true);
}

void Boid_Explosion::Update(const GameContext & gameContext)
{
	//update the frame
	m_FrameTimer -= gameContext.pTime->DeltaTime();
	if (m_FrameTimer <= 0)
	{
		m_FrameTimer = 0.05f;
		m_CurrentFrame++;
		if (m_CurrentFrame > m_NFrames)
			Destroy();
	}
}

void Boid_Explosion::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	switch (m_CurrentFrame)
	{
	case 0:
		SpriteRenderer::DrawSprite(m_Frame00_TextureID, m_Position, glm::vec2(30), m_Angle, glm::vec4(m_Color,1),glm::vec2(0.5f) ) ;
		break;
	case 1:
		SpriteRenderer::DrawSprite(m_Frame01_TextureID, m_Position, glm::vec2(30), m_Angle, glm::vec4(m_Color, 1), glm::vec2(0.5f));
		break;
	case 2:
		SpriteRenderer::DrawSprite(m_Frame02_TextureID, m_Position, glm::vec2(30), m_Angle, glm::vec4(m_Color, 1), glm::vec2(0.5f));
		break;	
	case 3:
		SpriteRenderer::DrawSprite(m_Frame03_TextureID, m_Position, glm::vec2(30), m_Angle, glm::vec4(m_Color, 1), glm::vec2(0.5f));
		break;
	default:
		break;
	}
}
