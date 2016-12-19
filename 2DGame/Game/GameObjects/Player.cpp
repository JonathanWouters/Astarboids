//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Player.h"

#include "Engine\Base\GameCore.h"
#include "Engine\Base\GameCore.h"
#include "Engine\Base\ContentManager.h"
#include "Engine\SceneGraph\GameScene.h"
#include "Engine\SceneGraph\GameObject.h"
#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Collisions\Collider.h"

#include "Bullet.h"
#include "Boid.h"
#include "PickupHealth.h"
#include "Wall.h"

#define PLAYER_IMG_LOCATION L"./Resources/Textures/Player.png"
#define HPBAR_IMG_LOCATION L"./Resources/Textures/HpBar.png"
#define HPBAR_BACKGROUND_IMG_LOCATION L"./Resources/Textures/HpBarBackground.png"
#define RECT_IMG_LOCATION L"./Resources/Textures/gray8px.png"

Player::Player(glm::vec2 position, float maxVelocity, float accelerationTime, float deaccelerationTime ) :
	m_MaxVelocity(maxVelocity),
	m_Acceleration(maxVelocity / accelerationTime),
	m_Deacceleration(maxVelocity / deaccelerationTime)
{
	m_Position = position;
}

Player::~Player()
{
	delete m_Collider;
}

void Player::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_TextureID = ContentManager::LoadTexture(PLAYER_IMG_LOCATION, true);

	m_Collider = new Collider(this, m_Size, m_Size);
	m_Collider->SetLayer( CollisionLayers::Player );

	m_HPTextureID = ContentManager::LoadTexture(HPBAR_IMG_LOCATION, true);
	m_HPBackgroundTextureID = ContentManager::LoadTexture(HPBAR_BACKGROUND_IMG_LOCATION, true);
	m_RectTextureID = ContentManager::LoadTexture(RECT_IMG_LOCATION, true);
}

void Player::Update(const GameContext & gameContext)
{

	//Get the move vector from input
	glm::vec2 moveInput = MoveInput(gameContext);

	Move( moveInput, gameContext.pTime->DeltaTime() );

	//rotate the player to the mouse
	RotateToMouse(gameContext);

	//Shoot
	if (gameContext.pInput->OnMouseButtonDown(SDL_BUTTON_LEFT))
		Shoot(gameContext);

	//Reduce invincibility timer
	UpdateInvincibility(gameContext.pTime->DeltaTime());
}

void Player::AddHealth(int hp) 
{ 
	m_Hits += hp; 
	m_Hits = std::min(m_Hits, m_MaxHits); 
}

void Player::SetFireMode(FireMode mode, int shots) 
{ 
	m_FireMode = mode; 
	m_MaxPowerUpShots = m_PowerUpShots = shots;
}

glm::vec2 Player::MoveInput(const GameContext & gameContext)
{
	glm::vec2 moveInput;
	if (gameContext.pInput->GetKey(SDL_SCANCODE_W))
		moveInput.y = 1;
	else if (gameContext.pInput->GetKey(SDL_SCANCODE_S))
		moveInput.y = -1;

	if (gameContext.pInput->GetKey(SDL_SCANCODE_A))
		moveInput.x = -1;
	else if (gameContext.pInput->GetKey(SDL_SCANCODE_D))
		moveInput.x = 1;

	//No input return vec2(0,0):
	if (moveInput.x == 0 && moveInput.y == 0)
		return glm::vec2(0);

	return glm::normalize( moveInput );
}

void Player::Move(const glm::vec2 & direction,float deltaTime)
{
	//No move input -> slow down
	if (direction == glm::vec2() && m_Velocity != glm::vec2())
	{
		glm::vec2 inverseVelocityDir = -glm::normalize(m_Velocity);

		float displacement = m_Deacceleration * deltaTime;
		if (glm::length2(m_Velocity) <  displacement * displacement)
			m_Velocity = glm::vec2();
		else
			m_Velocity += inverseVelocityDir * displacement;
	}
	//else speed up
	else
		m_Velocity += direction * m_Acceleration * deltaTime;

	//Clamp the velocity to the max velocity
	if (glm::length2(m_Velocity) >= m_MaxVelocity * m_MaxVelocity)
		m_Velocity = glm::normalize(m_Velocity) * m_MaxVelocity;

	//Move the player
	m_Position += m_Velocity * deltaTime;
}

void Player::UpdateInvincibility(float deltaTime)
{
	m_InvincibilityTimer -= deltaTime;
	if (m_InvincibilityTimer > 0.3f)
		m_CurrentColor = m_HitColor;
	else
		m_CurrentColor = m_Color;
}

void Player::RotateToMouse(const GameContext & gameContext)
{
	//Rotate
	glm::vec2 mouseWorldPos = gameContext.pCamera->ScreenToWorld(gameContext.pInput->GetGlMousePosition());
	m_Direction = -glm::normalize(m_Position - mouseWorldPos);
	m_Angle = -glm::orientedAngle(m_Direction, glm::vec2(0, 1));
}

void Player::Shoot(const GameContext & gameContext)
{
	switch (m_FireMode)
	{
	case FireMode::SINGLE:
		m_pParentGameScene->AddChild(std::shared_ptr<Bullet>(new Bullet(m_Position + m_Direction * m_Size/2.0f , m_Direction)));
		break;

	case FireMode::SPREAD:

		m_pParentGameScene->AddChild(std::shared_ptr<Bullet>(new Bullet(m_Position + m_Direction * m_Size / 2.0f, m_Direction)));
		m_pParentGameScene->AddChild(std::shared_ptr<Bullet>(new Bullet(m_Position + glm::vec2(m_Direction.y, -m_Direction.x)  * (m_Size / 2  + 10), m_Direction)));
		m_pParentGameScene->AddChild(std::shared_ptr<Bullet>(new Bullet(m_Position + glm::vec2(-m_Direction.y, m_Direction.x) *  (m_Size / 2  + 10), m_Direction)));

		m_PowerUpShots--;
		if (m_PowerUpShots <= 0)
			m_FireMode = FireMode::SINGLE;
		break;
	default:
		break;
	}

	//Shake camera
	gameContext.pCamera->ShakeCamera(0.2f, 4.0f);
}

void Player::Draw(const GameContext & gameContext)
{
	//Shadow
	SpriteRenderer::DrawSprite(m_TextureID, m_Position + glm::vec2(2,-2), glm::vec2(m_Size), m_Angle, glm::vec4(0.0f,0.0f,0.0f,0.5f), glm::vec2(0.5f));
	
	//Sprite
	SpriteRenderer::DrawSprite(m_TextureID, m_Position, glm::vec2(m_Size), m_Angle, m_CurrentColor, glm::vec2(0.5f));

	//HP Bar
	glm::vec2 barPosition = gameContext.pCamera->ScreenToWorld(glm::vec2(GameCore::GetGameSettings().Window.Width / 2, GameCore::GetGameSettings().Window.Height - 20));
	float barScale = m_Hits / static_cast<float>(m_MaxHits);
	SpriteRenderer::DrawSprite(m_HPBackgroundTextureID, barPosition, glm::vec2(512, 32), 0, glm::vec4(1), glm::vec2(0.5f));
	SpriteRenderer::DrawSprite(m_HPTextureID, barPosition, glm::vec2(512 * barScale, 32), 0, glm::vec4(1), glm::vec2(0.5f));

	//Power up bar
	if (m_FireMode != FireMode::SINGLE)
	{
		glm::vec2 fireModeBarPosition = m_Position + glm::vec2(0,-40);
		float fireModeBarScale = m_PowerUpShots / static_cast<float>(m_MaxPowerUpShots);

		SpriteRenderer::DrawSprite(m_RectTextureID, fireModeBarPosition, glm::vec2(100, 10), 0, glm::vec4(3), glm::vec2(0.5f));
		SpriteRenderer::DrawSprite(m_RectTextureID, fireModeBarPosition - glm::vec2(49,0), glm::vec2(98 * fireModeBarScale, 8), 0, glm::vec4(0,0,1,0.5f), glm::vec2(0, 0.5f));
	}
}

void Player::OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect)
{
	//Is hit by boid
	Boid* pBoid = dynamic_cast<Boid *>(other->GetGameObject());
	if (pBoid != nullptr && m_InvincibilityTimer <= 0)
	{
		//Shake camera
		gameContext.pCamera->ShakeCamera(0.2f, 5.0f);

		//Reset invincibilety timer
		m_InvincibilityTimer = 0.5f;

		//Decrease hits
		m_Hits--;
		if (m_Hits == 0)
				Destroy();
	}

	//Wall collision
	Wall* pWall = dynamic_cast<Wall *>(other->GetGameObject());
	if (pWall != nullptr)
	{
		float halfWidth = m_Size /2;
		float halfHeight = m_Size /2;

		if (hitRect.Width < hitRect.Height)
		{
			if ( m_Position.x + halfWidth == hitRect.GetRight() )
				m_Position.x -= hitRect.Width;
			else if (m_Position.x - halfWidth == hitRect.GetLeft())
				m_Position.x += hitRect.Width;
		}
		else
		{
			if (m_Position.y + halfHeight == hitRect.GetTop())
				m_Position.y -= hitRect.Height;
			else if (m_Position.y - halfHeight == hitRect.GetBottom())
				m_Position.y += hitRect.Height;
		}
	}	
}
