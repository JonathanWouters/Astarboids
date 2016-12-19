//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "Boid.h"

#include "Engine\Base\ContentManager.h"
#include "Engine\SceneGraph\GameScene.h"
#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Base\GameCore.h"
#include "Engine\Collisions\Collider.h"

#include "Bullet.h"
#include "Wall.h"
#include "Boid_Explosion.h"
#include "PickupHealth.h"
#include "PickupSpread.h"

#define BOID_IMG_LOCATION L"./Resources/Textures/Boid.png"

Boid::Boid()
{
}

Boid::~Boid()
{
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
	}

}

void Boid::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Load the texture
	m_TextureID = ContentManager::LoadTexture(BOID_IMG_LOCATION, true);
	
	//Load the collider
	m_pCollider = new Collider(this, m_Width,m_Height);
	m_pCollider->SetLayer(CollisionLayers::Enemies);

	m_SteerForceMultipliers.Alignment = 0.4f;
	m_SteerForceMultipliers.Cohesion = 0.2f;
	m_SteerForceMultipliers.Separation = 0.5f;

	//Set a random decision time so not every boid makes decisions at the same time
	m_DecisionTimer = Rand(1, m_DecisionTime);
}

void Boid::Update(const GameContext & gameContext)
{	
	//If the current behaviour =/= previous behaviour the behaviour has changed
	if (m_Behaviour != m_PreviousBehaviour)
		m_BehaviourChanged = true;
	else
		m_BehaviourChanged = false;
	m_PreviousBehaviour = m_Behaviour;


	switch (m_Behaviour)
	{
	case Behaviour::FLOCKING:
		FlockingBehaviour(gameContext.pTime->DeltaTime());
		break;
	case Behaviour::ANGRY:
		AngryBehaviour(gameContext.pTime->DeltaTime());
		break;
	default:
		break;
	}

	Move(gameContext.pTime->DeltaTime());
	UpdateColor(gameContext.pTime->DeltaTime());
}

void Boid::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Draw shadow
	SpriteRenderer::DrawSprite(m_TextureID, m_Position + glm::vec2(2,-2), glm::vec2(30), m_Angle, glm::vec4(0.0f, 0.0f, 0.0f, 0.5f), glm::vec2(0.5f));

	//Draw sprite
	SpriteRenderer::DrawSprite(m_TextureID, m_Position, glm::vec2(30), m_Angle, glm::vec4(m_CurrentColor,1.0f), glm::vec2(0.5f));
}

void Boid::Steer(glm::vec2 steerTarget, float weight,float deltaTime)
{
	if (steerTarget == glm::vec2(0) || weight == 0)
		return;

	glm::vec2 dir = glm::normalize(steerTarget);
	dir *= m_MaxSpeed;

	glm::vec2 steer = dir - m_Velocity;
	LimitVec2(steer, m_MaxSteeringForce * deltaTime);
	
	m_SteerForce += steer * weight;
}

//Returns true if a decision can be made
bool Boid::UpdateDecisionTimer(float deltaTime)
{
	m_DecisionTimer -= deltaTime;
	if (m_DecisionTimer <= 0)
	{
		m_DecisionTimer = m_DecisionTime;
		return true;
	}

	return false;
}

void Boid::UpdateColor(float deltaTime)
{
	if (m_ColorTimer > 0)
	{
		m_ColorTimer -= deltaTime;
		m_CurrentColor = m_HitColor;
	}
	else
		m_CurrentColor = m_Color;
}

void Boid::FlockingBehaviour(float deltaTime)
{
	bool makeDecision = UpdateDecisionTimer(deltaTime);

	if (m_BehaviourChanged)
	{
		m_Color = m_FlockingColor;
		m_MaxSpeed = 300.0f;

		m_SteerForceMultipliers.Alignment = 0.4f;
		m_SteerForceMultipliers.Cohesion = 0.3f;
		m_SteerForceMultipliers.Separation = 0.5f;
		m_SteerForceMultipliers.Seek = 0.0f;
		m_DesiredDistanceFromOthers = 40;
	}

	if (makeDecision)
	{
		//30% chance the boid will become angry and attack the player
		if (Chance(30))
			m_Behaviour = Behaviour::ANGRY;
	}
}

void Boid::AngryBehaviour(float deltaTime)
{
	if (m_BehaviourChanged)
	{
		m_Color = m_AngryColor;
		m_MaxSpeed = 600.0f;

		m_SteerForceMultipliers.Alignment = 0.0f;
		m_SteerForceMultipliers.Cohesion = 0.0f;
		m_SteerForceMultipliers.Separation = 0.8f;
		m_SteerForceMultipliers.Seek = 0.4f;
	}

	//Update target position
	if (!m_pTarget.expired())
		m_TargetPosition = m_pTarget.lock()->GetPosition();

	//if the target has been reached -> flock
	if (ReachedTarget())
		m_Behaviour = Behaviour::FLOCKING;

	bool makeDecision = UpdateDecisionTimer(deltaTime);
	if (makeDecision)
		m_Behaviour = Behaviour::FLOCKING;
}

void Boid::Move(float deltaTime)
{
	//Movement
	LimitVec2(m_OtherForces, m_MaxOtherForces);
	m_Velocity += m_SteerForce;
	m_Direction = glm::normalize(m_Velocity);
	m_Velocity += 10 * deltaTime;
	LimitVec2(m_Velocity, m_MaxSpeed);
	m_SteerForce = glm::vec2(0);

	m_Angle = -(glm::orientedAngle(m_Direction, glm::vec2(0, 1)));
	m_Position += (m_Velocity + m_OtherForces) * deltaTime;

	float friction = m_FrictionForce * deltaTime;

	if (glm::length2(m_OtherForces) > friction * friction)
		m_OtherForces -= glm::normalize(m_OtherForces) * friction;
	else
		m_OtherForces = glm::vec2(0);
}

void Boid::OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect)
{
	//Is hit by bullet
	Bullet* pBullet = dynamic_cast<Bullet *>(other->GetGameObject());
	if (pBullet != nullptr)
	{
		//Add Knockback
		m_OtherForces += pBullet->GetDirection() * 300.0f;

		//Hit timer
		m_ColorTimer = 0.15f;
		
		//Shake camera
		gameContext.pCamera->ShakeCamera(0.2f, 0.4f);

		//Decrease hits
		m_Hits--;

		//If died
		if (m_Hits == 0)
		{
			//add explosion effect
			std::shared_ptr<Boid_Explosion> explosion = std::shared_ptr < Boid_Explosion >(new Boid_Explosion(m_Position,m_Angle,m_Color) ) ;
			m_pParentGameScene->AddChild(explosion);

			//Drops
			if (Chance(m_DropChance) )
			{
				std::shared_ptr<PickupHealth> healthDrop = std::shared_ptr < PickupHealth >(new PickupHealth(m_Position));
				m_pParentGameScene->AddChild(healthDrop);
			}
			else if (Chance(m_DropChance) ) 
			{
				std::shared_ptr<PickupSpread> spreadDrop = std::shared_ptr < PickupSpread >(new PickupSpread(m_Position));
				m_pParentGameScene->AddChild(spreadDrop);
			}

			//Destroy object
			Destroy();
		}
	}

	//Wall collision
	Wall* pWall = dynamic_cast<Wall *>(other->GetGameObject());
	if (pWall != nullptr)
	{
		float halfWidth = m_Width / 2;
		float halfHeight = m_Height / 2;

		if (hitRect.Width < hitRect.Height)
		{
			if (m_Position.x + halfWidth == hitRect.GetRight())
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
