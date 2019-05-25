#pragma once

#include <glm/vec3.hpp>

class Texture {
public:
	virtual glm::vec3 Value(float u, float v, const glm::vec3& p) const = 0;
};