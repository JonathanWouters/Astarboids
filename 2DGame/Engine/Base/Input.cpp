//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Input.h"
#include "GameCore.h"

Input::Input()
{
}

Input::~Input()
{
	delete[] m_PreviousKeyboardState;
	delete[] m_CurrentKeyboardState;
}

void Input::Update()
{
	UpdateKeyboardState();

	UpdateMouseState();
}

void Input::UpdateMouseState()
{
	//Set previous mouse state equal to the current mouse state
	m_PreviousMouseState = m_CurrentMouseState;
	m_PreviousMousePos = m_CurrentMousePos;

	//Get the new mouse state
	int posX, posY;
	m_CurrentMouseState = SDL_GetMouseState(&posX, &posY);
	m_CurrentMousePos = glm::vec2(posX, posY);

	m_MouseMovement = m_PreviousMousePos - m_CurrentMousePos;
}

void Input::UpdateKeyboardState()
{
	//Set previous keyboard state equal to the current keyboard state
	delete[] m_PreviousKeyboardState;
	m_PreviousKeyboardState = m_CurrentKeyboardState;

	//Get the new keyboard state
	int arrayLenght = 0;
	const Uint8* state = SDL_GetKeyboardState(&arrayLenght);
	m_CurrentKeyboardState = new Uint8[arrayLenght];
	std::memcpy(m_CurrentKeyboardState, state, arrayLenght);
	
}

glm::vec2 Input::GetMouseMovement() const
{
	return m_MouseMovement;
}

glm::vec2 Input::GetMousePosition() const
{
	return m_CurrentMousePos;
}

glm::vec2 Input::GetGlMousePosition() const
{
	//Flip mouse coord from top left origin to bottom left origin
	glm::vec2 glMousePos = m_CurrentMousePos;
	glMousePos.y = GameCore::GetGameSettings().Window.Height - m_CurrentMousePos.y;
	return glMousePos;
}

bool Input::OnKeyDown(SDL_Scancode key) const
{
	if (m_CurrentKeyboardState == nullptr || m_PreviousKeyboardState == nullptr)
		return false;

	if (m_CurrentKeyboardState[key] && !m_PreviousKeyboardState[key])
		return true;

	return false;
}

bool Input::OnMouseButtonDown(int button) const
{
	if ( (m_CurrentMouseState & button) && !(m_PreviousMouseState & button) )
		return true;

	return false; 
}

bool Input::GetKey(SDL_Scancode key) const
{
	if ( m_CurrentKeyboardState == nullptr )
		return false;

	if (m_CurrentKeyboardState[key])
		return true;

	return false;
}

bool Input::GetMouseButton(int button) const
{
	if (m_CurrentMouseState & button)
	{
		return true;
	}
	return false;
}

bool Input::OnKeyUp(SDL_Scancode key) const
{
	if (m_CurrentKeyboardState == nullptr || m_PreviousKeyboardState == nullptr)
		return false;

	if (!m_CurrentKeyboardState[key] && m_PreviousKeyboardState[key])
		return true;

	return false;
}

bool Input::OnMouseButtonUp(int button) const
{
	if ( !(m_CurrentMouseState & button) && (m_PreviousMouseState & button))
	{
		return true;
	}
	return false;
}
