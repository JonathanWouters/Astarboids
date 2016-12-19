#pragma once
#include "..\Engine\SceneGraph\GameScene.h"

//Forward declarations
class Texture2D;
class Player;
class MousePoint;
class SpriteFont;
class Flock;
class Wall;
//

enum class GameState
{
	START,
	PLAYING,
	GAME_OVER
};

class Game : public GameScene
{
public:
	Game();
	virtual ~Game();

	virtual void Initialize(const GameContext & gameContext);
	virtual void Update(const GameContext & gameContext);
	virtual void PostUpdate(const GameContext & gameContext);
	virtual void Draw(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	void SetupGame();
	void DestroyGame();

	void LogFps(const GameContext & gameContext);
	void ClampCamera(int width, int height, const GameContext & gameContext);
	void DrawStart();
	void DrawGameOver();

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	//
	float m_FpsInterval = 1;
	std::wstring m_Fps = L"";

	//
	int m_WorldWidth = 1360;
	int m_WorldHeight = 1360;
	
	//Gameobjects
	std::weak_ptr<Flock> m_pFlock;
	std::weak_ptr<Player> m_pPlayer;
	std::weak_ptr<MousePoint> m_pMouse;

	std::weak_ptr<Wall> m_pWall0;
	std::weak_ptr<Wall> m_pWall1;
	std::weak_ptr<Wall> m_pWall2;
	std::weak_ptr<Wall> m_pWall3;

	std::weak_ptr<Wall> m_pBorderWall0;
	std::weak_ptr<Wall> m_pBorderWall1;
	std::weak_ptr<Wall> m_pBorderWall2;
	std::weak_ptr<Wall> m_pBorderWall3;

	GameState m_GameState = GameState::START;
	
	//Fonts
	SpriteFont* m_pFontConsolas32 = nullptr;
	SpriteFont* m_pFontConsolas72 = nullptr;
	SpriteFont* m_pFontCourierNew14 = nullptr;

	//Textures
	int m_TextureGrayID = 0;
};
