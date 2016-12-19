//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Collider.h"

#include "..\SceneGraph\GameObject.h"
#include "..\Base\ContentManager.h"
#include "..\Graphics\SpriteRenderer.h"

std::vector<Collider*> Collider::m_pColliders;
int Collider::m_DebugBoxTextureID = 0;

#define DEBUG_BOX_IMG_LOCATION L"./Resources/Textures/Debug_Box.png"


Collider::Collider(GameObject* parent, float width, float height, const glm::vec2 & offset) :
	m_pParent(parent),
	m_Offset(offset)
{
	m_Rect = Rect(parent->GetPosition(),width,height);
	//Add the collider to the static colliders vector
	m_pColliders.push_back(this);
}

Collider::~Collider()
{
	//Remove the collider from the colliders vector
	auto it = std::find(m_pColliders.begin(), m_pColliders.end(), this);
	if (it != m_pColliders.end() )
		m_pColliders.erase(it);
}

bool Collider::Collides(const Collider * other)
{
	//Test if the 2 colliders overlap
	return RectIntersectTest(GetRect(), other->GetRect());
}

bool Collider::Collides(const Collider * other, Rect & hitRect)
{
	if (RectIntersectTest(m_Rect, other->GetRect()))
	{
		//if there is an intersection calculate the intersecting rect
		float left = std::max(m_Rect.GetLeft(), other->GetRect().GetLeft() );
		float top = std::min(m_Rect.GetTop(), other->GetRect().GetTop());
		float bottom = std::max(m_Rect.GetBottom(), other->GetRect().GetBottom());
		float right = std::min(m_Rect.GetRight(), other->GetRect().GetRight());

		hitRect.Width = abs(right - left);
		hitRect.Height = abs(top - bottom);
		hitRect.Position = glm::vec2( left + hitRect.Width / 2.0f,
									  bottom + hitRect.Height / 2.0f);
		return true;
	}
	return false;
}

void Collider::DrawCollider()
{
	//Draws the collider (for debugging)
	SpriteRenderer::DrawSprite(m_DebugBoxTextureID, m_Rect.Position, glm::vec2(m_Rect.Width,m_Rect.Height), 0, glm::vec4(0.8f, 0, 0, 0.5f), glm::vec2(0.5f));
}

void Collider::UpdatePosition() 
{ 
	//update the position of the rect
	m_Rect.Position = m_pParent->GetPosition(); 
}

bool Collider::LineIntersect(const glm::vec2 & startPos, const glm::vec2 & endPos, HitInfo & hitinfo)
{
	//Update rect
	m_Rect.Position = m_pParent->GetPosition();

	//Algorithm used from noonat.github.io/intersect/ 
	
	//Vector from StartPos to EndPos
	glm::vec2 delta = endPos - startPos;
	
	//Scale to scale to 0 -> 1
	float scaleX = 1.0f / delta.x;
	float scaleY = 1.0f / delta.y;
	
	//Direction of the line
	int signX = static_cast<int>( glm::sign(scaleX) );
	int signY = static_cast<int>( glm::sign(scaleY) );
	
	//Half width / height
	float halfWidth = m_Rect.Width / 2.0f;
	float halfHeight = m_Rect.Height / 2.0f;
	
	//Time along line to nearest edge
	float nearTimeY = ( m_Rect.Position.y - signY * halfHeight -startPos.y ) * scaleY ;
	float nearTimeX = ( m_Rect.Position.x - signX * halfWidth  -startPos.x ) * scaleX ;
	
	//Time along line to furdest edge
	float farTimeX =  ( m_Rect.Position.x + signX * halfWidth  - startPos.x ) * scaleX ;
	float farTimeY =  ( m_Rect.Position.y + signY * halfHeight - startPos.y ) * scaleY ;
	
	//If the closest time of collision on either axis is further than the far time on the opposite axis, we can’t be colliding.
	if (nearTimeX > farTimeY || nearTimeY > farTimeX)
		return false;
	
	float nearTime = nearTimeX > nearTimeY ? nearTimeX : nearTimeY;
	float farTime =  farTimeX < farTimeY ? farTimeX : farTimeY;
	
	if (nearTime >= 1 || farTime <= 0)
		return false;
	
	hitinfo.pCollider = this;
	hitinfo.Position = startPos + Clamp(-nearTime, 0, 1) * delta;
	return true;
}

void Collider::CheckCollisions(const GameContext & gameContext)
{
	//If there are no colliders return
	if (m_pColliders.size() == 0)
		return;

	//Compare each collider to every other collider
	Rect hitRect;
	for (size_t i = 0; i < m_pColliders.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_pColliders.size(); j++)
		{		
			m_pColliders[i]->UpdatePosition();
			m_pColliders[j]->UpdatePosition();
			
			//Check for collisions
			if( m_pColliders[i]->Collides( m_pColliders[j], hitRect) )
			{
				//Notify both parent objects
				m_pColliders[i]->m_pParent->OnCollision(gameContext,m_pColliders[j],hitRect);
				m_pColliders[j]->m_pParent->OnCollision(gameContext,m_pColliders[i],hitRect);
				hitRect = Rect();
			}
		}
	}
}

bool Collider::RayCast(const glm::vec2 & position, const glm::vec2 & direction, HitInfo & hitinfo, float lenght, int ingnoreLayers)
{
	//Calculates end point and calls RayCast(pos, endpos ...)
	glm::vec2 endPoint = position + direction * lenght;
	return RayCast(position, endPoint, hitinfo, ingnoreLayers);
}

bool Collider::RayCast(const glm::vec2 & position, const glm::vec2 & endPosition, HitInfo & hitinfo, int ingnoreLayers)
{
	//If there are no colliders return
	if (m_pColliders.size() == 0)
		return false;

	HitInfo info;
	float minDistance = std::numeric_limits<float>().max();
	bool collision = false;

	for (Collider* collider : m_pColliders)
	{
		//if the collider is in the ingnorelayers continue to next collider
		if ((collider->GetLayer() & ingnoreLayers) != 0)
		{
			continue;
		}

		//If the line intersect with the collider check if it is closer to the start position than the closest hit so far.
		if (collider->LineIntersect(position, endPosition, hitinfo))
		{
			float distanceSqr = glm::distance2(info.Position, position);
			if (distanceSqr < minDistance)
			{
				info = hitinfo;
				minDistance = distanceSqr;
				collision = true;
			}
		}
	}

	hitinfo = info;
	return collision;
}

bool Collider::RectIntersectTest(const Rect & r1, const Rect & r2)
{
	//Checks if the to rects intersect at any point
	return !(r1.GetRight() < r2.GetLeft() || r1.GetLeft() > r2.GetRight() || r1.GetTop() < r2.GetBottom() || r1.GetBottom() > r2.GetTop());
}



