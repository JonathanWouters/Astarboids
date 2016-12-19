//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "PickupSpread.h"

#include "Engine\Base\ContentManager.h"
#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Collisions\Collider.h"

#include "Player.h"

#define PICKUP_SPREAD_IMG_LOCATION L"./Resources/Textures/PowerUpPickup.png"


PickupSpread::PickupSpread(const glm::vec2 & position)
{
	m_Position = position;
}

PickupSpread::~PickupSpread()
{
	delete m_pCollider;
}

void PickupSpread::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pCollider = new Collider(this, 60,60);
	m_pCollider->SetLayer(CollisionLayers::None);

	m_TexturID = ContentManager::LoadTexture(PICKUP_SPREAD_IMG_LOCATION, true);
}

void PickupSpread::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SpriteRenderer::DrawSprite(m_TexturID, m_Position, glm::vec2(30), 0, glm::vec4(0.8, 0.8f, 0, 1), glm::vec2(0.5f));
}

void PickupSpread::OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect)
{
	UNREFERENCED_PARAMETER(hitRect);
	UNREFERENCED_PARAMETER(gameContext);

	Player* pPlayer = dynamic_cast<Player *>(other->GetGameObject());
	if (pPlayer != nullptr)
	{
		pPlayer->SetFireMode(FireMode::SPREAD, 20);
		Destroy();
	}
}
