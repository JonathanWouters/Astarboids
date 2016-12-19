#pragma once
#include <map>

class Input
{
public:
	Input();
	~Input();
	
	void Update();

	glm::vec2 GetMouseMovement() const;
	glm::vec2 GetMousePosition() const;
	glm::vec2 GetGlMousePosition() const;

	bool OnKeyDown(SDL_Scancode key) const;
	bool OnMouseButtonDown(int button) const;

	bool GetKey(SDL_Scancode key) const;
	bool GetMouseButton(int button) const;

	bool OnKeyUp(SDL_Scancode key) const;
	bool OnMouseButtonUp(int button) const;

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	void UpdateMouseState();
	void UpdateKeyboardState();

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	Uint8 *m_CurrentKeyboardState = nullptr;
	Uint8 *m_PreviousKeyboardState = nullptr;

	Uint32 m_CurrentMouseState = 0;
	Uint32 m_PreviousMouseState = 0;

	glm::vec2 m_MouseMovement = glm::vec2(0);
	glm::vec2 m_PreviousMousePos = glm::vec2(0);
	glm::vec2 m_CurrentMousePos = glm::vec2(0);
};

