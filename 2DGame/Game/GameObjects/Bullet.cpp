//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "Bullet.h"

#include "Engine\Base\ContentManager.h"
#include "Engine\Base\GameCore.h"
#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Collisions\Collider.h"

#include "Player.h"

#define BULLET_IMG_LOCATION L"./Resources/Textures/Bullet.png"

Bullet::Bullet(const glm::vec2 & position, const glm::vec2 & direction):
	m_Direction(direction)
{
	m_Position = position;
}

Bullet::~Bullet()
{
	delete m_pCollider;
}

void Bullet::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_TextureID = ContentManager::LoadTexture(BULLET_IMG_LOCATION, true);
	m_Angle = -(glm::orientedAngle(m_Direction, glm::vec2(0, 1)));

	m_pCollider = new Collider(this, m_Size.x , m_Size.y);
}

void Bullet::Update(const GameContext & gameContext)
{
	//update the position and the life time of the bullet
	m_Position += m_Direction * m_Speed * gameContext.pTime->DeltaTime();
	m_LifeTime -= gameContext.pTime->DeltaTime();
	if (m_LifeTime <= 0)
		Destroy();

}

void Bullet::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SpriteRenderer::DrawSprite(m_TextureID, m_Position, m_Size, m_Angle, glm::vec4(1.0f,0.5f,0.0f,1), glm::vec2(0.5f));
}

void Bullet::OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect)
{
	UNREFERENCED_PARAMETER(hitRect);
	UNREFERENCED_PARAMETER(gameContext);

	//if bullet hit player don't destroy the bullet
	Player* player = dynamic_cast<Player*>(other->GetGameObject());
	Bullet* bullet = dynamic_cast<Bullet*>(other->GetGameObject());
	if (player == nullptr && bullet == nullptr)
		Destroy();
}
