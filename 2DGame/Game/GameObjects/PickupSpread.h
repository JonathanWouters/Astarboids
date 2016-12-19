#pragma once
#include "Engine\SceneGraph\GameObject.h"

class Collider;


class PickupSpread : public GameObject
{
public:
	PickupSpread(const glm::vec2& position);
	~PickupSpread();

	void Initialize(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);
	void OnCollision(const GameContext & gameContext, const Collider* other,const Rect& hitRect);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	//Texture
	unsigned int m_TexturID = 0;
	
	//Collider
	Collider* m_pCollider = nullptr;
};
