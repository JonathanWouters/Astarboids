#pragma once
class GameObject;
class Collider;
struct GameContext;


enum class CollisionLayers : int
{
	None		=	1 << 0,
	Default		=	1 << 1,
	Player		=	1 << 2,
	Enemies		=	1 << 3,
	Obstacles	=	1 << 4
};

inline int operator~ (CollisionLayers a) { return ~(int)a; }
inline int operator| (CollisionLayers a, CollisionLayers b) { return ((int)a | (int)b); }
inline int operator& (CollisionLayers a, CollisionLayers b) { return ((int)a & (int)b); }
inline int operator^ (CollisionLayers a, CollisionLayers b) { return ((int)a ^ (int)b); }
inline int& operator|= (CollisionLayers& a, CollisionLayers b) { return ((int&)a |= (int)b); }
inline int& operator&= (CollisionLayers& a, CollisionLayers b) { return ((int&)a &= (int)b); }
inline int& operator^= (CollisionLayers& a, CollisionLayers b) { return ((int&)a ^= (int)b); }

struct HitInfo
{
	Collider* pCollider;
	glm::vec2 Position;
};

struct Rect
{
	Rect()
	{}

	Rect(glm::vec2 position, float width, float heigth) :
		Position(position),
		Width(width),
		Height(heigth)
	{
	
	}

	glm::vec2 Position;
	float Width;
	float Height;

	float GetLeft() const { return Position.x - Width / 2.0f; }
	float GetRight() const { return Position.x + Width / 2.0f; }
	float GetBottom() const { return Position.y - Height / 2.0f; }
	float GetTop() const { return Position.y + Height / 2.0f;	}
};

class Collider
{

public:
	Collider(GameObject* parent, float width, float height, const glm::vec2 & offset = glm::vec2(0));
	~Collider();
	void SetLayer(int layer) { m_Layer = layer; }
	void SetLayer(CollisionLayers layer) { m_Layer = static_cast<int>(layer); }
	int GetLayer() const { return m_Layer; }

	bool Collides(const Collider * other);
	bool Collides(const Collider * other, Rect & hitRect);

	void DrawCollider();


	GameObject* GetGameObject() const { return m_pParent; }
	const Rect& GetRect() const { return m_Rect; }
	void UpdatePosition();

	bool LineIntersect(const glm::vec2 & startPos, const glm::vec2 & endPos, HitInfo & hitinfo);

	static void CheckCollisions(const GameContext & gameContext);
	static bool RayCast(const glm::vec2 & position, const glm::vec2 & direction, HitInfo & hitinfo, float lenght,int ingnoreLayers = 0);
	static bool RayCast(const glm::vec2 & position, const glm::vec2 & endPosition,HitInfo & hitinfo, int ingnoreLayers = 0);
	static bool RectIntersectTest(const Rect & r1, const Rect & r2);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	//Colliders
	static std::vector<Collider*> m_pColliders;
	
	//Texture
	static int m_DebugBoxTextureID;

	//Parent game object
	GameObject* m_pParent;

	//Dimensions
	glm::vec2 m_Offset;
	Rect m_Rect;

	//Collison Layer
	int m_Layer = static_cast<int>( CollisionLayers::Default );
};
