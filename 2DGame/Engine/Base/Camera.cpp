//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Camera.h"
#include "GameCore.h"
#include "GeneralStructs.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(const GameContext& gameContext)
{
	UpdateScreenShake(gameContext);

	//Make Projection Matrix
	GameSettings settings = GameCore::GetGameSettings();
	int windowWidth = settings.Window.Width / 2;
	int windowHeight = settings.Window.Height / 2;
	m_Projection = glm::ortho(-static_cast<float>(windowWidth), static_cast<float>(windowWidth), -static_cast<float>(windowHeight), static_cast<float>(windowHeight), m_NearPlane, m_FarPlane);

	//Make view matrix
	m_CameraTransform = glm::translate( glm::vec3(-m_Position + m_ShakeOffset,0) );
	
}

glm::mat4 Camera::GetCameraTransform() const 
{ 
	return m_CameraTransform; 
}

glm::mat4 Camera::GetProjection() const 
{ 
	return m_Projection; 
}

void Camera::SetPosition(const glm::vec2 & position) 
{ 
	m_Position = position; 
}

glm::vec2 Camera::GetPosition() const 
{ 
	return m_Position; 
}

void Camera::ShakeCamera(float time, float distance)
{
	m_ShakeTimer = time;
	m_MaxShakeTimer = time;
	m_ShakeDistance = distance;
	m_IsShaking = true;
	m_ShakeOffset = glm::vec2(0);
}

glm::vec2 Camera::ScreenToWorld(const glm::vec2 & screencoord) 
{ 
	GameSettings settings = GameCore::GetGameSettings();
	int windowWidth = settings.Window.Width;
	int windowHeight = settings.Window.Height;

	glm::vec2 worldPos = screencoord + m_Position - glm::vec2(windowWidth/2.0f, windowHeight/2.0f);
	return worldPos; 
}

glm::vec2 Camera::WorldToScreen(const glm::vec2 & worldPos)
{
	//Get Window Width and Height fro GameSettings
	GameSettings settings = GameCore::GetGameSettings();
	int windowWidth = settings.Window.Width;
	int windowHeight = settings.Window.Height;

	glm::vec2 screencoord = worldPos - m_Position + glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f);
	return worldPos;
}

void Camera::UpdateScreenShake(const GameContext & gameContext)
{
	//Check if the screen is shaking
	if (m_IsShaking)
	{
		//Reduce the shake timer
		m_ShakeTimer -= gameContext.pTime->DeltaTime();
		
		//if the timet is 0 reset the screen
		if (m_ShakeTimer <= 0)
		{
			m_IsShaking = false;
			m_ShakeOffset = glm::vec2(0);
		}
		else
		{
			//Move the shake offset based on a sine wave
			float percent = 1 - (m_ShakeTimer / m_MaxShakeTimer);
			float anglePercent = glm::two_pi<float>() * 2 * percent;
			m_ShakeOffset = glm::vec2(std::sin(anglePercent)) * m_ShakeDistance;
		}
	}
	else
		m_ShakeOffset = glm::vec2(0,0);
}
