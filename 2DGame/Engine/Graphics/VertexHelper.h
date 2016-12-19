#pragma once
#include "stdafx.h"

struct VertexPosCol
{
public:
	VertexPosCol() {};
	VertexPosCol(glm::vec3 pos, glm::vec4 col) :
		Position(pos), Color(col) {}

	glm::vec3 Position;
	glm::vec4 Color;
};

struct VertexPosColTex
{
public:
	VertexPosColTex() {};
	VertexPosColTex(glm::vec3 pos, glm::vec4 col,glm::vec2 uv) :
		Position(pos), Color(col),UV(uv) {}

	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 UV;
};

struct VertexPosTex
{
public:
	VertexPosTex() {};
	VertexPosTex(glm::vec3 pos, glm::vec2 uv) :
		Position(pos), UV(uv) {}

	glm::vec3 Position;
	glm::vec2 UV;
};

struct VertexPosNormCol
{
public:
	VertexPosNormCol() {};
	VertexPosNormCol(glm::vec3 pos, glm::vec3 normal, glm::vec4 col) :
		Position(pos), Color(col), Normal(normal) {}

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Color;
};


struct VertexPosNormTex
{
public:

	VertexPosNormTex() {};
	VertexPosNormTex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoord) :
		Position(pos), Normal(norm), TexCoord(texCoord) {}

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

