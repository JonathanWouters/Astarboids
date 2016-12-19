#pragma once
#include "GeneralStructs.h"

class SpriteRenderer;
class GameCore
{
public:
	GameCore();
	~GameCore();

	static const GameSettings& GetGameSettings() { return m_GameSettings; }
	void RunGame();

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	void Initialize();
	void InitWindow();
	void DestroyWindow();
	void GameLoop();

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	int m_WindowHandle = 0;
	SDL_GLContext m_GLContext;
	static GameSettings m_GameSettings;

	GameScene* m_pCurrentScene = nullptr;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameCore(const GameCore &obj);
	GameCore& operator=(const GameCore& obj);
};

