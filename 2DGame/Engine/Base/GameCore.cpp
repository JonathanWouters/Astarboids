//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameCore.h"

#include "..\Graphics\SpriteRenderer.h"
#include "..\Graphics\TextRenderer.h"
#include "..\Graphics\ShaderProgram.h"
#include "..\Graphics\Texture2D.h"
#include "..\Graphics\VertexHelper.h"

#include "ContentManager.h"
#include "Game\Game.h"


GameSettings GameCore::m_GameSettings = GameSettings();

GameCore::GameCore()
{
	Logger::Initialize();
}

GameCore::~GameCore()
{
	SDL_Quit();

	Logger::Release();
	SpriteRenderer::Release();
	TextRenderer::Release();
	ContentManager::Release();
	delete(m_pCurrentScene);
}

void GameCore::RunGame()
{
	//INITIALIZATION
	Initialize();

	m_pCurrentScene = new Game();
	m_pCurrentScene->RootInitialize(&m_GLContext);

	//Set backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// GAMELOOP
	// Loops until the event "SDL_QUIT" gets triggerd
	while (m_GameSettings.Window.WindowHandle)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			//If event is "SDL_QUIT" destroy the game window
			if (event.type == SDL_QUIT )
			{
				
				DestroyWindow();
				return;
			}
		}

		//Clear the backbuffer
		glClear(GL_COLOR_BUFFER_BIT);

		GameLoop();

		//Swap buffers
		SDL_GL_SwapWindow(m_GameSettings.Window.WindowHandle);
	}

}

void GameCore::Initialize()
{
	//Set OPENGL version (current 4.5.0)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Initialize the window
	InitWindow();

	//Initialize Glew
	glewExperimental = GL_TRUE;
	GLenum glewInitResult = glewInit();

	//Check if glew is initialized
	if (GLEW_OK != glewInitResult) {
		Logger::LogGLEnumError(glewInitResult);
		exit(EXIT_FAILURE);
	}


	//Initialize the sprite renderer
	SpriteRenderer::Initialize();

	//Initialize the text renderer
	TextRenderer::Initialize();

	//Print what version of OpenGl we're using
	Logger::LogGLEnumInfo(GL_VERSION);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //Colour that the backbuffer will clear to
}

void GameCore::InitWindow()
{
	//Initialize SDL Window and store it in m_GameSettings
	m_GameSettings.Window.WindowHandle = SDL_CreateWindow(m_GameSettings.Window.Title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_GameSettings.Window.Width,
		m_GameSettings.Window.Height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	
	//Create GL Context
	m_GLContext = SDL_GL_CreateContext(m_GameSettings.Window.WindowHandle);
}

void GameCore::DestroyWindow()
{
	//Delete the context
	SDL_GL_DeleteContext(m_GLContext);

	//Delete the window
	SDL_DestroyWindow(m_GameSettings.Window.WindowHandle);
	m_GameSettings.Window.WindowHandle = nullptr;
}

void GameCore::GameLoop()
{
	m_pCurrentScene->RootUpdate();
	m_pCurrentScene->RootDraw();
}