//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "GameScene.h"
#include "GameObject.h"

#include "..\Graphics\SpriteRenderer.h"
#include "..\Graphics\TextRenderer.h"

#include "..\Collisions\Collider.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//Cleanup GameContext
	delete m_GameContext.pTime;
	delete m_GameContext.pInput;
	delete m_GameContext.pCamera;
	
	m_pChildren.clear();
}

void GameScene::RootInitialize(SDL_GLContext* context)
{
	m_GameContext.GLContext = context;
	m_GameContext.pTime = new GameTimer();
	m_GameContext.pInput = new Input();
	m_GameContext.pCamera = new Camera();

	//Scene initialize
	Initialize(m_GameContext);
}

void GameScene::RootUpdate()
{
	//Add all objects that need to be added
	AddObjects();

	//Update GameContext
	m_GameContext.pTime->Update();
	m_GameContext.pInput->Update();
	m_GameContext.pCamera->Update(m_GameContext);

	//Scene Update
	Update(m_GameContext);

	//Game Objects Update
	for (std::shared_ptr<GameObject> pChild : m_pChildren)
	{
		if (!pChild->IsDestroyed())
			pChild->RootUpdate(m_GameContext);
	}

	//Check all collisions
	Collider::CheckCollisions(m_GameContext);

	//Post update
	PostUpdate(m_GameContext);

	//Post update objects
	for (std::shared_ptr<GameObject> pChild : m_pChildren)
	{
		if (!pChild->IsDestroyed())
			pChild->RootPostUpdate(m_GameContext);
	}

	//Remove all destroyed objects
	RemoveObjects();
	
}

void GameScene::RootDraw()
{
	//Scene Draw
	Draw(m_GameContext);
	
	//Object Draw
	for (std::shared_ptr<GameObject> pChild : m_pChildren)
	{
		if ( !pChild->IsDestroyed() )
			pChild->RootDraw(m_GameContext);
	}

	//Draw sprites
	SpriteRenderer::Draw(m_GameContext);

	//Draw Text
	TextRenderer::Draw(m_GameContext);
}

void GameScene::AddObjects()
{
	for (size_t i = 0; i < m_pObjectsToAdd.size(); i++ )
	{
		#if _DEBUG
			if (m_pObjectsToAdd[i]->m_pParentGameScene)
			{
				if (m_pObjectsToAdd[i]->m_pParentGameScene == this)
					Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to this GameScene");
				else
					Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to another GameScene. Detach it from it's current scene before attaching it to another one.");
				return;
			}
		#endif

		//Set the parent scene to this scene
		m_pObjectsToAdd[i]->m_pParentGameScene = this;
		m_pObjectsToAdd[i]->Initialize(m_GameContext);
		//Add the object to the scene
		m_pChildren.push_back(m_pObjectsToAdd[i]);
	}

	m_pObjectsToAdd.clear();
}

void GameScene::RemoveObjects()
{
	for (auto pObj : m_pObjectsToRemove)
	{

		auto it = std::find(m_pChildren.begin(), m_pChildren.end(), pObj );

		#if _DEBUG
				if (it == m_pChildren.end())
				{
					Logger::LogWarning(L"GameScene::RemoveChild > GameObject to remove is not attached to this GameScene!");
					continue;
				}
		#endif
		
		//remove from vector
		m_pChildren.erase(it);

		//Set parent scene to null
		pObj->m_pParentGameScene = nullptr;
	}
	
	m_pObjectsToRemove.clear();
}

void GameScene::AddChild(const std::shared_ptr<GameObject> & object)
{
	m_pObjectsToAdd.push_back(object);
}

void GameScene::RemoveChild(const std::shared_ptr<GameObject> & object)
{
	m_pObjectsToRemove.push_back(object );
}

void GameScene::RemoveAllChilderen()
{
	m_pObjectsToRemove = m_pChildren;
}


