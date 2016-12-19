//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameTimer.h"

GameTimer::GameTimer()
{
	m_LastTime = m_CurrentTime = SDL_GetPerformanceCounter();
}

GameTimer::~GameTimer()
{
}

float GameTimer::GetFPS()
{
	return m_Fps;
}

float GameTimer::DeltaTime()
{
	return m_DeltaTime;
}

void GameTimer::Update()
{
	//Update the timers
	m_CurrentTime = SDL_GetPerformanceCounter();
	m_DeltaTime = (float)((m_CurrentTime - m_LastTime) * 1.0 / SDL_GetPerformanceFrequency());
	m_LastTime = m_CurrentTime;


	if (m_DeltaTime != 0)
		m_Fps = (1.0f / (m_DeltaTime ) );
}
