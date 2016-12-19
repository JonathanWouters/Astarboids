#pragma once
#include "Engine\SceneGraph\GameObject.h"

class Wall : public GameObject
{
public:
	Wall(const glm::vec2 & position,float width,float height,const glm::vec4 & color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) );
	~Wall();

	virtual void Initialize(const GameContext & gameContext);
	virtual void Update(const GameContext & gameContext);
	virtual void Draw(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	
	//Dimensions
	float m_Width = 0;
	float m_Height = 0;

	//Collider
	Collider* m_pCollider = nullptr;

	//Texture
	unsigned int m_TextureID = 0;
	
	//Colour
	glm::vec4 m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
};
