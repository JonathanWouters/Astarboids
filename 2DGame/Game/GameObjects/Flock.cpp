//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Flock.h"

#include "Engine\Base\GameCore.h"
#include "Engine\SceneGraph\GameScene.h"
#include "Engine\Helpers\CoordinateHelpers.h"
#include "Engine\Collisions\Collider.h"

#include "Boid.h"

Flock::Flock()
{
}

Flock::~Flock()
{
	m_pBoids.clear();
}

void Flock::SetTarget(const std::weak_ptr<GameObject>& target)
{
	m_pTarget = target;
}

void Flock::Initialize(const GameContext & gameContext)
{	
	UNREFERENCED_PARAMETER(gameContext);
	
	SpawnBoidsAtRandomPos(5);

	//resize the grid to fit all grid cells
	m_Grid.resize(m_GridResolution * m_GridResolution);
}

void Flock::Update(const GameContext & gameContext)
{
	//Clear empty pointers from vector
	m_pBoids.erase( std::remove_if( m_pBoids.begin(), m_pBoids.end(), 
					[](const std::weak_ptr<Boid>& boid) { return boid.expired(); }),
					m_pBoids.end());

	//Spawn new boids every m_SpawnTimerInterval
	m_SpawnTimer -= gameContext.pTime->DeltaTime();
	if (m_SpawnTimer <= 0 && m_pBoids.size() < 200 )
	{
		m_SpawnTimer = m_SpawnTimerInterval;
		SpawnBoidsAtRandomPos(m_NBoidsToSpawn);
		m_NBoidsToSpawn++;
	}

	//Map boids to grid
	for (std::weak_ptr<Boid> pBoid : m_pBoids)
	{		
		m_Grid[GetGridCell(pBoid.lock()->GetPosition())].push_back(pBoid);
	}

	//For all boids in this system
	for (std::weak_ptr<Boid> boid : m_pBoids)
	{
		auto pBoid = boid.lock();

		glm::vec2 seek = Seek(pBoid->GetPosition(), pBoid->GetTargetPos() );

		//Avoid walls and obstacles
		glm::vec2 AvoidWalls = glm::vec2(0);
		if (pBoid->GetVelocity() != glm::vec2(0))
		{
			glm::vec2 direction = glm::normalize(pBoid->GetVelocity());
			HitInfo info;
			AvoidWalls = Collider::RayCast(pBoid->GetPosition(), direction, info, 50, ~CollisionLayers::Obstacles) ? -pBoid->GetVelocity() : glm::vec2(0);
		}

		
		//glm::vec2 avoid = Avoid(m_pMousePoint->GetRadius() * 5, pBoid->GetPosition(), m_pMousePoint->GetPosition());
		glm::vec2 cohesion = glm::vec2();
		glm::vec2 seperation = glm::vec2();
		glm::vec2 alignment = glm::vec2();
		
		int nBoidsInAlignmentRadius = 0;
		int nBoidsInSeparationRadius = 0;
		int nBoidsInCohesionRadius = 0;
		
		unsigned int gridCell = GetGridCell(pBoid->GetPosition());

		for (int row = -1; row <= 1; row++)
		{
			for (int col = -1; col <= 1; col++)
			{
				unsigned int cell = gridCell + (m_GridResolution * row + col);
				if (cell >= m_GridResolution * m_GridResolution)
					continue;

				//Calculate steering
				for (std::weak_ptr<Boid> pNeighbourBoid : m_Grid[cell] )
				{
					// Variables
					glm::vec2 delta = pNeighbourBoid.lock()->GetPosition() - pBoid->GetPosition();
					glm::vec2 direction = glm::normalize(delta);
					float distanceSqrt = glm::length2(delta);

					// Forces witouth the current boid
					if (pBoid != pNeighbourBoid.lock() )
					{
						alignment += Alignment(distanceSqrt,100, pNeighbourBoid.lock()->GetVelocity(), nBoidsInAlignmentRadius);
						seperation += Seperation(distanceSqrt, 500, delta,pBoid->GetDisiredDistance(), nBoidsInSeparationRadius);
						cohesion += Cohesion(distanceSqrt, 100, delta, nBoidsInCohesionRadius);
					}
				}
			}
		}
		
		//Average Forces
		if (nBoidsInAlignmentRadius > 0)
			alignment /= (float)nBoidsInAlignmentRadius;
		
		if (nBoidsInSeparationRadius > 0)
			cohesion /= (float)nBoidsInSeparationRadius;
		
		if (nBoidsInCohesionRadius > 0)
		cohesion /= (float)nBoidsInCohesionRadius;
		
		
		//Steer
		float deltaTime = gameContext.pTime->DeltaTime();
		pBoid->Steer(seperation, pBoid->GetSteerForceMultipliers().Separation, deltaTime);
		pBoid->Steer(cohesion, pBoid->GetSteerForceMultipliers().Cohesion, deltaTime);
		pBoid->Steer(alignment, pBoid->GetSteerForceMultipliers().Alignment, deltaTime);
		pBoid->Steer(seek, pBoid->GetSteerForceMultipliers().Seek, deltaTime);
		pBoid->Steer(AvoidWalls, 100, deltaTime);
	}
	
	//clear grid
	for (size_t i = 0; i < m_Grid.size(); i++)
	{
		m_Grid[i].clear();
	}
}

void Flock::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void Flock::SpawnBoid(const glm::vec2 & position, const glm::vec2 & velocity)
{
	auto pboid = std::shared_ptr<Boid>(new Boid() );
	pboid->SetPosition(position);
	pboid->SetVelocity(velocity);

	if (!m_pTarget.expired())
		pboid->SetTarget(m_pTarget);

	AddChild( pboid );
	m_pBoids.push_back(pboid);
}

void Flock::SpawnBoidsAtRandomPos(size_t nBoids)
{
	//Get the window width and height
	int windowHeight = GameCore::GetGameSettings().Window.Height;
	int windowWidth = GameCore::GetGameSettings().Window.Width;

	//Spawn boids with random position and velocity
	for (size_t i = 0; i < nBoids; i++)
	{
		//Position
		int x = std::rand() % windowWidth - windowWidth / 2;
		int y = std::rand() % windowHeight - windowHeight / 2;

		//velocity direction
		float vx = (std::rand() % 200 - 100.0f) / 100.0f;
		float vy = (std::rand() % 200 - 100.0f) / 100.0f;

		SpawnBoid(glm::vec2(x, y), glm::normalize(glm::vec2(vx, vy)) * 180.0f);
	}
}

unsigned int Flock::GetGridCell(const glm::vec2 & position) const
{
	int windowHeight = GameCore::GetGameSettings().Window.Height;
	int windowWidth = GameCore::GetGameSettings().Window.Width;
	
	glm::vec2 positionTopRight = CenterToTopRightCoord(position, glm::vec2(windowWidth, windowHeight));

	unsigned int column = unsigned int(positionTopRight.x / (windowWidth / m_GridResolution));
	unsigned int row = unsigned int(positionTopRight.y / (windowHeight / m_GridResolution));
	
	column = std::min(column, m_GridResolution - 1);
	row = std::min(row, m_GridResolution - 1);
	return m_GridResolution * row + column;
}

glm::vec2 Flock::Alignment(float distanceSqrt, float radius,const glm::vec2 & velocity,int & nBoidsInRadius)
{	
	//Get velocity of boid in radius
	glm::vec2 alignment = glm::vec2(0);
	if (distanceSqrt < radius * radius)
	{
		alignment += velocity;
		nBoidsInRadius++;
	}

	return alignment;
}

glm::vec2 Flock::Seperation(float distanceSqrt, float radius,const glm::vec2 & delta,float desiredDistance,int & nBoidsInRadius)
{
	glm::vec2 seperation = glm::vec2(0);
	
	if (distanceSqrt < radius * radius)
	{
		if (distanceSqrt < desiredDistance * desiredDistance)
		{
			if (distanceSqrt == 0)
				seperation -= glm::vec2(desiredDistance, 0);
			else
				seperation -= delta / distanceSqrt;

			nBoidsInRadius++;
		}
	}

	return seperation;
}

glm::vec2 Flock::Cohesion(float distanceSqrt, float radius,const glm::vec2 & delta, int & nBoidsInRadius)
{
	glm::vec2 centerOfMass;

	if (distanceSqrt < radius * radius)
	{
		centerOfMass += delta;
		nBoidsInRadius++;
	}

	return centerOfMass;
}

glm::vec2 Flock::Seek(const glm::vec2 & position, const glm::vec2 & target)
{
	return target - position;
}

glm::vec2 Flock::Avoid(float range, const glm::vec2 & position, const glm::vec2 & target)
{
	glm::vec2 deltaPos = position - target;
	float distanceSqrt = glm::length2(deltaPos);

	if (distanceSqrt < range * range)
	{
		return deltaPos;
	}

	return glm::vec2(0);
}