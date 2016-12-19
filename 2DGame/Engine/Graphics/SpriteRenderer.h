#pragma once
#include "..\Base\GeneralStructs.h"

class ShaderProgram;
class Texture2D;

struct SpriteVertex
{
	glm::vec4 TransformData;	//PosX, PosY, Depth (PosZ), Rotation
	glm::vec4 TransformData2;	//PivotX, PivotY, SizeX, SizeY
	glm::vec4 Color;
	unsigned int TextureID;
	bool IsInWorldspace;

};

inline bool operator==(const SpriteVertex& lhs, const SpriteVertex& rhs) 
{
	return (lhs.TextureID == rhs.TextureID &&
			lhs.Color == rhs.Color &&
			lhs.TransformData == rhs.TransformData &&
			lhs.TransformData2 == rhs.TransformData2);
}

class SpriteRenderer
{
public:
	static void Initialize();
	static void Release();
	static void DrawSprite(unsigned int textureID, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec4 color, glm::vec2 pivot,bool worldspace = true);
	static void Draw(const GameContext& gameContext);

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	SpriteRenderer() {};
	~SpriteRenderer() {};

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	static void InitRenderData();
	static void UpdateBuffer();

	static ShaderProgram* m_pShaderProgram;
	static GLuint m_VBO;
	static GLuint m_VAO;
	static unsigned int m_BufferSize;

	static std::vector<SpriteVertex> m_Sprites;
	static bool m_IsInitialized;
};

