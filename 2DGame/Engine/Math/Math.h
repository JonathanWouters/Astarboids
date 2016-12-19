#pragma once

#include <algorithm>
#include <glm.hpp>
#include <gtx/vector_angle.hpp>

//Consts
static const float Pi = 3.14159265358979323846264338327950f;

//Functions
inline float Clamp(float value, float min, float max)
{
	return std::min(max, std::max(min, value));
}

inline float Lerp(float t, float min, float max)
{
	return t * (max - min) + min;
}

inline float Rand(float min, float max)
{
	int value = rand() % 1000;
	return Lerp(value / 999.f, min, max);
}

inline void LimitVec2(glm::vec2 & vec, float force)
{
	if (glm::length2(vec) > force * force)
		vec = glm::normalize(vec) * force;
}

inline bool Chance(int chancePercentage)
{
	return (Rand(0, 100) < chancePercentage);
}