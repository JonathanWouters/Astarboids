//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Wall.h"

#include "Engine\Collisions\Collider.h"
#include "Engine\Base\ContentManager.h"
#include "Engine\Graphics\SpriteRenderer.h"

#define WALL_IMG_LOCATION L"./Resources/Textures/gray8px.png"

Wall::Wall(const glm::vec2 & position, float width, float height, const glm::vec4 & color) :
	m_Width(width),
	m_Height(height),
	m_Color(color)
{
	m_Position = position;
}

Wall::~Wall()
{
	delete m_pCollider;
}

void Wall::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_TextureID = ContentManager::LoadTexture(WALL_IMG_LOCATION, true);
	m_pCollider = new Collider(this, m_Width, m_Height);
	m_pCollider->SetLayer(CollisionLayers::Obstacles);
}

void Wall::Update(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void Wall::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Shadow
	SpriteRenderer::DrawSprite(m_TextureID, m_Position + glm::vec2(10, -10), glm::vec2(m_Width, m_Height), 0, m_Color * glm::vec4(1, 1, 1, 0.5f), glm::vec2(0.5f));

	//Wall
	SpriteRenderer::DrawSprite(m_TextureID, m_Position, glm::vec2(m_Width,m_Height), 0, m_Color, glm::vec2(0.5f));


}

