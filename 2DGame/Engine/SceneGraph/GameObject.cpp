//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameObject.h"
#include "GameScene.h"

void GameObject::RootUpdate(const GameContext & gameContext) 
{
	//Add all objects that need to be added
	AddChildren(gameContext);

	//Update child objects
	for each (auto child in m_Children)
	{
		child->Update(gameContext);
	}

	//Update
	Update(gameContext);


}

void GameObject::RootPostUpdate(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Remove child objects that need to be removed
	RemoveChildren();
}

void GameObject::RootDraw(const GameContext & gameContext) 
{
	//Draw chis objects
	for each (auto child in m_Children)
	{
		child->Draw(gameContext);
	}

	//Call draw
	Draw(gameContext);
}


void GameObject::Destroy()
{
	if (m_IsDestroyed == true)
		return;

	m_IsDestroyed = true;
	
	//kill the childeren
	for each (auto child in m_Children)
	{
		child->Destroy();
	}
	m_Children.clear();

	
	//remove from parent object
	if (m_pParentObject != nullptr)
	{
		m_pParentObject->RemoveChild(shared_from_this());
		m_pParentGameScene = nullptr;
	}
	
	//remove from scene
	if (m_pParentGameScene != nullptr)
		m_pParentGameScene->RemoveChild(shared_from_this());
}


void GameObject::RemoveChildren()
{
	for (auto pObj : m_ChildrenToRemove )
	{

		auto it = std::find(m_Children.begin(), m_Children.end(), pObj);

#if _DEBUG
		if (it == m_Children.end())
		{
			Logger::LogWarning(L"GameObject::RemoveChildren > GameObject to remove is not attached to this GameObject!");
			continue;
		}
#endif

		//remove from vector
		m_Children.erase(it);

		//Set parent scene to null
		pObj->m_pParentGameScene = nullptr;
	}

	m_ChildrenToRemove.clear();
}

void GameObject::AddChildren(const GameContext & gameContext)
{
	for (size_t i = 0; i < m_ChildrenToAdd.size(); i++)
	{
#if _DEBUG
		if (m_ChildrenToAdd[i]->m_pParentGameScene)
		{
			Logger::LogWarning(L"GameObject::AddChild > GameObject is already attached to a GameScene");
			return;
		}

		if (m_ChildrenToAdd[i]->m_pParentObject)
		{
			Logger::LogWarning(L"GameObject::AddChild > GameObject is already attached to a GameObject");
			return;
		}
#endif

		//Set the parent scene to this scene
		m_ChildrenToAdd[i]->m_pParentObject = this;
		m_ChildrenToAdd[i]->m_pParentGameScene = m_pParentGameScene;
		m_ChildrenToAdd[i]->Initialize(gameContext);
		//Add the object to the scene
		m_Children.push_back(m_ChildrenToAdd[i]);
	}

	m_ChildrenToAdd.clear();
}

void GameObject::AddChild(const std::shared_ptr<GameObject> & child)
{
	//Add child to a vector , Child will be added when AddChilderen gets called
	m_ChildrenToAdd.push_back(child);
}

void GameObject::RemoveChild(const std::shared_ptr<GameObject> & child)
{
	//Add child to a vector , Child will be added when RemoveChilderen gets called
	m_ChildrenToRemove.push_back(child);
}
