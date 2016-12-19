#pragma once
#include "Engine\SceneGraph\GameObject.h"

class Collider;

enum class FireMode
{
	SINGLE,
	SPREAD
};

class Player : public GameObject
{
public:
	Player(glm::vec2 position, float maxVelocity, float accelerationTime = 0.1f, float deaccelerationTime = 0.01f );
	~Player();

	void Initialize(const GameContext & gameContext);
	void Update(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);
	void OnCollision(const GameContext & gameContext, const Collider* other,const Rect& hitRect);

	glm::vec2 GetDirection() { return m_Direction; }
	Collider* GetCollider() const { return m_Collider; }

	void AddHealth(int hp);
	void SetFireMode(FireMode mode, int shots);


private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	glm::vec2 MoveInput(const GameContext & gameContext);
	void Move(const glm::vec2 & direction,float deltaTime);
	void UpdateInvincibility(float deltaTime);
	void RotateToMouse(const GameContext & gameContext);
	void Shoot(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	
	//Textures
	unsigned int m_TextureID = 0;
	unsigned int m_HPTextureID = 0;
	unsigned int m_HPBackgroundTextureID = 0;
	unsigned int m_RectTextureID = 0;
	
	//Collider
	Collider* m_Collider = nullptr;

	//
	float m_Size = 40.0f;
	float m_Angle = 0;
	
	//Movement
	glm::vec2 m_Velocity = glm::vec2(0);
	glm::vec2 m_Direction = glm::vec2(0, 1);

	float m_MaxVelocity = 0;
	float m_Acceleration = 0;
	float m_Deacceleration = 0;

	//Color
	float m_InvincibilityTimer = 0;
	glm::vec4 m_Color = glm::vec4(0, 1, 0, .5f);
	glm::vec4 m_CurrentColor = glm::vec4(0, 1, 0, .5f);
	glm::vec4 m_HitColor = glm::vec4(1, 1, 1, 1);

	//Hits
	unsigned int m_MaxHits = 10;
	unsigned int m_Hits = 10;
	
	//Powerups
	int m_PowerUpShots = 20;
	int m_MaxPowerUpShots = 20;
	FireMode m_FireMode = FireMode::SINGLE;
};
