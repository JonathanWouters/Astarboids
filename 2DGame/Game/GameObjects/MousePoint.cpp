//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "MousePoint.h"

#include "Engine\Base\ContentManager.h"
#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Base\GameCore.h"

#define POINT_IMG_LOCATION L"./Resources/Textures/Point.png"
MousePoint::MousePoint()
{

}

MousePoint::~MousePoint()
{

}

void MousePoint::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_TextureID = ContentManager::LoadTexture(POINT_IMG_LOCATION, true);
}

void MousePoint::Update(const GameContext & gameContext)
{
	//Set position
	m_Position = gameContext.pCamera->ScreenToWorld( gameContext.pInput->GetGlMousePosition() );
	
	//On Button down
	if (gameContext.pInput->OnMouseButtonDown(SDL_BUTTON_LEFT))
	{
		m_Color.w = 0.1f;
		m_Radius = 10.0f;
	}

	//On button up
	if (gameContext.pInput->OnMouseButtonUp(SDL_BUTTON_LEFT))
	{
		m_Color.w = 0.4f;
		m_Radius = 12.0f;
	}

}

void MousePoint::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SpriteRenderer::DrawSprite(m_TextureID, m_Position, glm::vec2(m_Radius*2), 0, m_Color, glm::vec2(0.5f));
}
