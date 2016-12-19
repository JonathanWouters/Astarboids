#pragma once
#include "Engine\SceneGraph\GameObject.h"

class Boid_Explosion : public GameObject
{
public:
	Boid_Explosion(const glm::vec2 position, float angle, const glm::vec3 & color);
	~Boid_Explosion();

	void Initialize(const GameContext & gameContext);
	void Update(const GameContext & gameContext);
	void Draw(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	//Frames
	float m_FrameTimer = 0.05f;
	int m_CurrentFrame = 0;
	int m_NFrames = 3;

	//Orientation
	float m_Angle;

	//Colour
	glm::vec3 m_Color;


	//Textures
	unsigned int m_Frame00_TextureID = 0;
	unsigned int m_Frame01_TextureID = 0;
	unsigned int m_Frame02_TextureID = 0;
	unsigned int m_Frame03_TextureID = 0;
};
