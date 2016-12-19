//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Game.h"

#include "Engine\Base\GameCore.h"
#include "Engine\Base\ContentManager.h"
#include "Engine\Base\GameCore.h"

#include "Engine\Graphics\SpriteRenderer.h"
#include "Engine\Graphics\TextRenderer.h"

#include "Engine\Helpers\CoordinateHelpers.h"

//Game objects
#include "GameObjects\Flock.h"
#include "GameObjects\Player.h"
#include "GameObjects\MousePoint.h"
#include "GameObjects\Wall.h"

#define FPS_COUNTER 1
Game::Game()
{
}


Game::~Game()
{
}

void Game::Initialize(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Hide the cursor
	SDL_ShowCursor(SDL_DISABLE);
	
	//Load the fonts
	unsigned int fontID = ContentManager::LoadSpriteFont(L"./Resources/Fonts/CourierNew_24.fnt");
	m_pFontCourierNew14 = ContentManager::GetSpriteFont(fontID);

	fontID = ContentManager::LoadSpriteFont(L"./Resources/Fonts/Consolas_32.fnt");
	m_pFontConsolas32 = ContentManager::GetSpriteFont(fontID);

	fontID = ContentManager::LoadSpriteFont(L"./Resources/Fonts/Consolas_72.fnt");
	m_pFontConsolas72 = ContentManager::GetSpriteFont(fontID);

	m_TextureGrayID = ContentManager::LoadTexture(L"./Resources/Textures/gray8px.png",true);
}

void Game::SetupGame()
{
	//Add walls
	auto wall = std::shared_ptr<Wall>(new Wall(glm::vec2(-150,-150), 100, 100));
	m_pWall0 = wall;
	AddChild(wall);

	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(150, -150), 100, 100));
	m_pWall1 = wall;
	AddChild(wall);

	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(150, 150), 100, 100));
	m_pWall2 = wall;
	AddChild(wall);

	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(-150, 150), 100, 100));
	m_pWall3 = wall;
	AddChild(wall);

	//ADD Border
	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(0, -640), 1280, 50));
	m_pBorderWall0 = wall;
	AddChild(wall);

	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(0, 640), 1280, 50));
	m_pBorderWall1 = wall;
	AddChild(wall);

	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(640, 0), 50, 1280));
	m_pBorderWall2 = wall;
	AddChild(wall);

	wall = std::shared_ptr<Wall>(new Wall(glm::vec2(-640, 0), 50, 1280));
	m_pBorderWall3 = wall;
	AddChild(wall);

	//Add player
	auto player = std::shared_ptr<Player>(new Player(glm::vec2(0), 500.0f));
	m_pPlayer = player;
	AddChild(player);
	
	//Add Flock
	auto flock = std::shared_ptr<Flock>(new Flock() );
	flock->SetTarget( player );
	m_pFlock = flock;
	AddChild(flock);

	//Add mouse indicator
	auto mouse = std::shared_ptr<MousePoint>(new MousePoint());
	m_pMouse = mouse;
	AddChild(mouse);
}

void Game::DestroyGame()
{
	RemoveAllChilderen();
}

void Game::Update(const GameContext & gameContext)
{
	switch (m_GameState)
	{
	case GameState::START:
		//If space is down start the game
		if (gameContext.pInput->OnKeyDown(SDL_SCANCODE_SPACE))
		{
			m_GameState = GameState::PLAYING;
			SetupGame();
		}
		break;
	case GameState::PLAYING:
		//if the player died end the game
		if (m_pPlayer.expired())
		{
			m_GameState = GameState::GAME_OVER;
			DestroyGame();
		}
		break;
	case GameState::GAME_OVER:

		if (gameContext.pInput->OnKeyDown(SDL_SCANCODE_SPACE))
		{
			m_GameState = GameState::PLAYING;
			SetupGame();
		}
		break;
	default:
		break;
	}
}

void Game::PostUpdate(const GameContext & gameContext)
{
	switch (m_GameState)
	{
	case GameState::START:
		break;
	case GameState::PLAYING:
		if (m_pPlayer.expired())
		{
			m_GameState = GameState::GAME_OVER;
			DestroyGame();
			break;
		}

		//Set the camera
		gameContext.pCamera->SetPosition(m_pPlayer.lock()->GetPosition());
		ClampCamera(m_WorldWidth, m_WorldHeight, gameContext);
		break;
	default:
		break;
	}
}

void Game::ClampCamera(int width, int height, const GameContext & gameContext)
{
	int halfWidth = (width - GameCore::GetGameSettings().Window.Width) / 2;
	int halfHeight = (height - GameCore::GetGameSettings().Window.Height) / 2;

	//Clamp camera to world border
	if (gameContext.pCamera->GetPosition().x < -halfWidth)
		gameContext.pCamera->SetPosition(glm::vec2(-halfWidth, gameContext.pCamera->GetPosition().y));

	else if (gameContext.pCamera->GetPosition().x > halfWidth)
		gameContext.pCamera->SetPosition(glm::vec2(halfWidth, gameContext.pCamera->GetPosition().y));

	if (gameContext.pCamera->GetPosition().y < -halfHeight)
		gameContext.pCamera->SetPosition(glm::vec2(gameContext.pCamera->GetPosition().x, -halfHeight));

	else if (gameContext.pCamera->GetPosition().y > halfHeight)
		gameContext.pCamera->SetPosition(glm::vec2(gameContext.pCamera->GetPosition().x, halfHeight));
}

void Game::Draw(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

#ifdef _DEBUG
	//render the fps in top right screen
	LogFps(gameContext);
	glm::vec2 windowDimensions = glm::vec2(GameCore::GetGameSettings().Window.Width, GameCore::GetGameSettings().Window.Height);
	TextRenderer::DrawText(m_pFontCourierNew14, L"FPS: " + m_Fps, TopRightToCenterCoord(glm::vec2(10), windowDimensions), glm::vec4(1));
#endif // DEBUG


	switch (m_GameState)
	{
	case GameState::START:
		DrawStart();
		break;
	case GameState::GAME_OVER:
		DrawGameOver();
		break;
	default:
		break;
	}
}

void Game::LogFps(const GameContext & gameContext)
{
	m_FpsInterval += gameContext.pTime->DeltaTime();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;

		m_Fps = std::to_wstring(gameContext.pTime->GetFPS());
		std::size_t comma = m_Fps.find(L".");
		m_Fps = m_Fps.substr(0, comma + 3);
	}
}

void Game::DrawStart()
{
	glm::vec2 windowDimensions = glm::vec2(GameCore::GetGameSettings().Window.Width, GameCore::GetGameSettings().Window.Height);

	//Title
	TextRenderer::DrawText(m_pFontConsolas72, L"Asterboids", glm::vec2(-155, 260), glm::vec4(1));
	SpriteRenderer::DrawSprite(m_TextureGrayID, TopRightToCenterCoord(glm::vec2(0, 60), windowDimensions), glm::vec2(1280, 130), 0, glm::vec4(1), glm::vec2(0), false);
	//Controls
	TextRenderer::DrawText(m_pFontCourierNew14, L"Use WASD(qwerty) / ZQSD(azerty) to move", glm::vec2(-265, 100), glm::vec4(1));
	TextRenderer::DrawText(m_pFontCourierNew14, L"Use LMB to shoot", glm::vec2(-265, 70), glm::vec4(1));
	TextRenderer::DrawText(m_pFontCourierNew14, L"Avoid the enemies for as long as possible", glm::vec2(-265, 40), glm::vec4(1));

	//Press space
	TextRenderer::DrawText(m_pFontConsolas32, L"Press Space to start", glm::vec2(-150, -100), glm::vec4(1));

	//Credits
	TextRenderer::DrawText(m_pFontCourierNew14, L"Made by Jonathan Wouters", glm::vec2(-155, -300), glm::vec4(1));
	SpriteRenderer::DrawSprite(m_TextureGrayID, TopRightToCenterCoord(glm::vec2(0, 655), windowDimensions), glm::vec2(1280, 40), 0, glm::vec4(1), glm::vec2(0), false);
}

void Game::DrawGameOver()
{
	glm::vec2 windowDimensions = glm::vec2(GameCore::GetGameSettings().Window.Width, GameCore::GetGameSettings().Window.Height);

	//Title
	TextRenderer::DrawText(m_pFontConsolas72, L"Asterboids", glm::vec2(-155, 260), glm::vec4(1));
	SpriteRenderer::DrawSprite(m_TextureGrayID, TopRightToCenterCoord(glm::vec2(0, 60), windowDimensions), glm::vec2(1280, 130), 0, glm::vec4(1), glm::vec2(0), false);

	TextRenderer::DrawText(m_pFontConsolas72, L"GAME OVER", glm::vec2(-155, 0), glm::vec4(1));

	//Press space
	TextRenderer::DrawText(m_pFontConsolas32, L"Press Space to start", glm::vec2(-150, -100), glm::vec4(1));

	//Credits
	TextRenderer::DrawText(m_pFontCourierNew14, L"Made by Jonathan Wouters", glm::vec2(-155, -300), glm::vec4(1));
	SpriteRenderer::DrawSprite(m_TextureGrayID, TopRightToCenterCoord(glm::vec2(0, 655), windowDimensions), glm::vec2(1280, 40), 0, glm::vec4(1), glm::vec2(0), false);

}
