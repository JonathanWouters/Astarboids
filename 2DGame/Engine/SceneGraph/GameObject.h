#pragma once
#include "..\Base\GeneralStructs.h"

class Collider;
struct Rect;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() {};
	virtual ~GameObject() {};

	virtual void Initialize(const GameContext & gameContext) { UNREFERENCED_PARAMETER(gameContext); };
	virtual void Update(const GameContext & gameContext){ UNREFERENCED_PARAMETER(gameContext); };
	virtual void Draw(const GameContext & gameContext){ UNREFERENCED_PARAMETER(gameContext); };

	virtual void OnCollision(const GameContext & gameContext,const Collider* other,const Rect& hitRect) { UNREFERENCED_PARAMETER(gameContext); UNREFERENCED_PARAMETER(other); UNREFERENCED_PARAMETER(hitRect)};

	virtual void Destroy();
	bool IsDestroyed() { return m_IsDestroyed; }

	void AddChild(const std::shared_ptr<GameObject> & child);
	void RemoveChild(const std::shared_ptr<GameObject>& child);
	
	glm::vec2 GetPosition() const { return m_Position; }
	void SetPosition(glm::vec2 position) { m_Position = position; }
private:
	friend class GameScene;

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	void RootUpdate(const GameContext & gameContext);
	void RootPostUpdate(const GameContext & gameContext);
	void RootDraw(const GameContext & gameContext);

	void RemoveChildren();
	void AddChildren(const GameContext & gameContext);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	bool m_IsDestroyed = false;
	
	std::vector<std::shared_ptr<GameObject>> m_ChildrenToRemove;
	std::vector<std::shared_ptr<GameObject>> m_ChildrenToAdd;

protected:
	GameScene* m_pParentGameScene = nullptr;
	GameObject* m_pParentObject = nullptr;
	glm::vec2 m_Position = glm::vec2(0, 0);
	std::vector<std::shared_ptr<GameObject>> m_Children;
};
