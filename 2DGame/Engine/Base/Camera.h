#pragma once
#include <gtx\matrix_transform_2d.hpp>
struct GameContext;

class Camera
{
public:
	Camera();
	~Camera();

	void Update(const GameContext& gameContext);
	
	glm::mat4 GetCameraTransform() const;
	glm::mat4 GetProjection() const;
	
	void SetPosition(const glm::vec2 & position);
	glm::vec2 GetPosition() const;;
	
	void ShakeCamera(float time, float distance);
	
	glm::vec2 ScreenToWorld(const glm::vec2& screencoord);
	glm::vec2 Camera::WorldToScreen(const glm::vec2 & worldPos);

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	void UpdateScreenShake(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	glm::mat4 m_CameraTransform;
	glm::mat4 m_Projection;

	float m_FarPlane = 1.0f;
	float m_NearPlane = -1.0f;
	
	glm::vec2 m_Position;
	float m_Rotation;
	
	// Screen shake
	bool m_IsShaking = false;
	float m_MaxShakeTimer = 0;
	float m_ShakeTimer = 0;
	float m_ShakeDistance = 0;
	glm::vec2 m_ShakeOffset;

};
