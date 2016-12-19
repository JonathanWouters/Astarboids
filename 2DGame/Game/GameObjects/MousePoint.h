#pragma once
#include "Engine\SceneGraph\GameObject.h"

class MousePoint : public GameObject
{
public:
	MousePoint();
	~MousePoint();

	void Initialize(const GameContext & gameContext);
	void Update(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);

	float GetRadius() const { return m_Radius; };
private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	
	//Texture
	unsigned int m_TextureID = 0;

	//Radius
	float m_Radius = 12.0f;

	//Colour
	glm::vec4 m_Color = glm::vec4(1, 1, 1, 0.4f);
};
