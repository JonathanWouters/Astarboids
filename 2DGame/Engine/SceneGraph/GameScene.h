#pragma once
#include "..\Base\GeneralStructs.h"

class GameObject;

class GameScene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual void Initialize(const GameContext & gameContext) = 0;
	virtual void Update(const GameContext & gameContext) = 0;
	virtual void PostUpdate(const GameContext & gameContext) = 0;
	virtual void Draw(const GameContext & gameContext) = 0;

	void AddChild(const std::shared_ptr<GameObject>& object);
	void RemoveChild(const std::shared_ptr<GameObject>& object);
	void RemoveAllChilderen();

private:
	friend class GameCore;

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	void RootInitialize(SDL_GLContext* context);
	void RootUpdate();
	void RootDraw();

	void AddObjects();
	void RemoveObjects();

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	GameContext m_GameContext;

	std::vector<std::shared_ptr<GameObject>> m_pObjectsToAdd;
	std::vector<std::shared_ptr<GameObject>> m_pObjectsToRemove;
	std::vector<std::shared_ptr<GameObject>> m_pChildren;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameScene(const GameScene &obj);
	GameScene& operator=(const GameScene& obj);
};