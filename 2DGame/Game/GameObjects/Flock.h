#pragma once
#include "Engine\SceneGraph\GameObject.h"

//Forward declarations
class Boid;

class Flock : public GameObject
{
public:
	Flock();
	virtual ~Flock();

	void SetTarget(const std::weak_ptr<GameObject> & target);
	void Initialize(const GameContext & gameContext);
	void Update(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	void SpawnBoid(const glm::vec2 & position, const glm::vec2 & velocity);
	void SpawnBoidsAtRandomPos(size_t nBoids);
	unsigned int GetGridCell(const glm::vec2 &position) const;
	
	//Steering functions
	glm::vec2 Alignment(float distanceSqrt, float radius, const glm::vec2 & velocity, int & nBoidsInRadius);
	glm::vec2 Seperation(float distanceSqrt, float radius, const glm::vec2 & delta, float desiredDistance, int & nBoidsInRadius);
	glm::vec2 Cohesion(float distanceSqrt, float radius, const glm::vec2 & delta, int & nBoidsInRadius);
	glm::vec2 Seek(const glm::vec2 & position, const glm::vec2 & target);
	glm::vec2 Avoid(float range, const glm::vec2 & position, const glm::vec2 & target);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	//List of all boids in the system
	std::vector<std::weak_ptr<Boid>> m_pBoids;

	//All boids mapped to a gridcell
	std::vector<std::vector<std::weak_ptr<Boid>>> m_Grid;
	unsigned int m_GridResolution = 20;

	//
	float m_SpawnTimer = 5;
	const float m_SpawnTimerInterval = 5.0f;
	int m_NBoidsToSpawn = 5;

	// Target
	std::weak_ptr<GameObject> m_pTarget;
};
