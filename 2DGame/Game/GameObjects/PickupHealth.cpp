//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "PickupHealth.h"

#include "Engine\Base\ContentManager.h"
#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Collisions\Collider.h"

#include "Player.h"

#define PICKUP_IMG_LOCATION L"./Resources/Textures/Pickup_Health.png"


PickupHealth::PickupHealth(const glm::vec2 & position)
{
	m_Position = position;
}

PickupHealth::~PickupHealth()
{
	delete m_pCollider;
}

void PickupHealth::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pCollider = new Collider(this,60,60);
	m_pCollider->SetLayer(CollisionLayers::None);

	m_TexturID = ContentManager::LoadTexture(PICKUP_IMG_LOCATION, true);
}

void PickupHealth::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SpriteRenderer::DrawSprite(m_TexturID, m_Position, glm::vec2(30), 0, glm::vec4(0, 0.8f, 0, 1), glm::vec2(0.5f));
}

void PickupHealth::OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect)
{
	UNREFERENCED_PARAMETER(hitRect);
	UNREFERENCED_PARAMETER(gameContext);
	Player* pPlayer = dynamic_cast<Player *>(other->GetGameObject());
	if (pPlayer != nullptr)
	{
		pPlayer->AddHealth(1);
		Destroy();
	}
}
