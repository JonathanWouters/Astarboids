#pragma once
#include "stdafx.h"
#include "GameTimer.h"
#include "Input.h"
#include "Camera.h"
struct GameSettings
{
public:
	GameSettings() :
		Window(WindowSettings())
	{}

#pragma region WINDOW_SETTINGS
	struct WindowSettings
	{
		WindowSettings() :
			Width(1280),
			Height(720),
			AspectRatio(Width / (float)Height),
			Title("2D GAME"),
			WindowHandle(0)
		{
		}

		int Width;
		int Height;
		float AspectRatio;
		std::string Title;
		SDL_Window* WindowHandle;
	}Window;
#pragma endregion WINDOW_SETTINGS

};

struct GameContext
{
public:
	SDL_GLContext GLContext;
	GameTimer* pTime;
	Input* pInput;
	Camera* pCamera;
};