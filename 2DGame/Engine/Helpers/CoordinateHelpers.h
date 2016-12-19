#pragma once
#include "stdafx.h"

//Converts a coordinate from center coordinates (0,0) = center of the screen to TopRightCoord (0,0) is top right of the screen
inline glm::vec2 CenterToTopRightCoord(const glm::vec2 & coord, const glm::vec2 & dimensions)
{
	glm::vec2 dimensionsFlipped = glm::vec2(dimensions.x, dimensions.y);
	glm::vec2 coordFlipped = glm::vec2(coord.x, -coord.y);
	return coordFlipped + dimensionsFlipped / 2.0f;
}

// Converts from (0,0) is top right of screen to (0,0) is center of screen
inline glm::vec2 TopRightToCenterCoord(const glm::vec2 & coord, const glm::vec2 & dimensions)
{
	glm::vec2 dimensionsFlipped = glm::vec2(dimensions.x, -dimensions.y);
	glm::vec2 coordFlipped = glm::vec2(coord.x, -coord.y);
	return coordFlipped - dimensionsFlipped / 2.0f;
}
