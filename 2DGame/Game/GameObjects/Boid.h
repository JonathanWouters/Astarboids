#pragma once
#include "Engine\SceneGraph\GameObject.h"

//Forward declarations
class Collider;

struct SteerForceMultipliers
{
	float Seek;
	float Avoid;
	float Alignment;
	float Cohesion;
	float Separation;
};

enum class Behaviour
{
	FLOCKING,
	ANGRY
};

class Boid : public GameObject
{
public:
	Boid();
	virtual ~Boid();

	void Initialize(const GameContext & gameContext);
	void Update(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);
	void OnCollision(const GameContext & gameContext, const Collider* other, const Rect& hitRect);

	void AddVelocity(const glm::vec2& velocity) { m_Velocity += velocity; }
	void SetVelocity(const glm::vec2& velocity) { m_Velocity = velocity; }
	void SetTarget(const std::weak_ptr<GameObject> target) { m_pTarget = target; }

	void Steer(glm::vec2 steerTarget, float weight, float deltaTime);

	glm::vec2 GetVelocity() const { return m_Velocity; };
	float GetMaxSpeed() const { return m_MaxSpeed; }
	float GetMaxSteer() const { return m_MaxSteeringForce; }
	float GetDisiredDistance() const { return m_DesiredDistanceFromOthers; }
	glm::vec2 GetTargetPos() const { return m_TargetPosition; }
	SteerForceMultipliers GetSteerForceMultipliers() const { return m_SteerForceMultipliers; }

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	bool ReachedTarget() const { return (glm::length2(m_Position - m_TargetPosition) < 400.0f); }
	bool UpdateDecisionTimer(float deltaTime);
	void UpdateColor(float deltaTime);
	void FlockingBehaviour(float deltaTime);
	void AngryBehaviour(float deltaTime);
	void Move(float deltaTime);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	
	//Texture
	unsigned int m_TextureID = 0;
	
	//Collider
	Collider* m_pCollider = nullptr;

	//Dimensions
	float m_Width = 30;
	float m_Height = 30;
	float m_Angle = 0.0f;

	//Movement
	glm::vec2 m_Direction = glm::vec2(0, -1);
	float m_MaxSpeed = 300.0f;
	glm::vec2 m_Velocity = glm::vec2(0);
	glm::vec2 m_SteerForce;
	float m_MaxSteeringForce = 2500.0f;
	SteerForceMultipliers m_SteerForceMultipliers;
	glm::vec2 m_OtherForces;
	float m_MaxOtherForces = 1000.0f;
	float m_FrictionForce = 700.0f;
	float m_DesiredDistanceFromOthers = 40.0f;

	//Target
	std::weak_ptr<GameObject> m_pTarget;
	glm::vec2 m_TargetPosition = glm::vec2(0);

	//Colors
	glm::vec3 m_CurrentColor = glm::vec3(0.5f);
	glm::vec3 m_Color = glm::vec3(0.5f);
	glm::vec3 m_AngryColor = glm::vec3(0.8f, 0, 0);
	glm::vec3 m_FlockingColor = glm::vec3(0.5f);
	glm::vec3 m_HitColor = glm::vec3(1, 1, 1);
	float m_ColorTimer;

	//If timer reaches zero a new decision can be made
	float m_DecisionTimer = 5;
	float m_DecisionTime = 5;

	//Hit Points
	unsigned int m_Hits = 2;
	unsigned int m_MaxHits = 2;

	//Behaviour
	Behaviour m_Behaviour = Behaviour::FLOCKING;
	Behaviour m_PreviousBehaviour = Behaviour::FLOCKING;
	bool m_BehaviourChanged = false;

	//Drops
	int m_DropChance = 5;
};
