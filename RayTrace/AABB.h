#pragma once

#include "Ray.h"

class AABB {
private:
	glm::vec3 min;
	glm::vec3 max;
	
public:
	AABB()
		:min(glm::vec3(0.0f, 0.0f, 0.0f)), max(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	
	AABB(const glm::vec3& minv, const glm::vec3& maxv) 
		:min(minv), max(maxv) {}

	bool IsHit(const Ray& r, float t_min, float t_max) const;

	glm::vec3 GetMin() const { return min; }
	glm::vec3 GetMax() const { return max; }
};