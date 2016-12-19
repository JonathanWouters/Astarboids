#pragma once
#include "Engine\SceneGraph\GameObject.h"

class Collider;

class Bullet : public GameObject
{
public:
	Bullet(const glm::vec2 & position,const glm::vec2 & direction);
	virtual ~Bullet();

	void Initialize(const GameContext & gameContext);
	void Update(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);
	void OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect);

	glm::vec2 GetDirection() const { return m_Direction;}

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	// Texture
	unsigned int m_TextureID = 0;

	//
	glm::vec2 m_Size =glm::vec2(30.0f);
	float m_Angle = 0.0f;
	float m_LifeTime = 5.0f;

	// Collider
	Collider* m_pCollider;
	
	//Movement
	float m_Speed = 1200.0f;
	glm::vec2 m_Direction;


};
