#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Ray {

private:
	
	glm::vec3 origin;
	glm::vec3 dir;

public:
	
	Ray()
		:origin(glm::vec3(0, 0, 0)), dir(glm::vec3(0, 0, 0)) {}

	Ray(const glm::vec3& ori, const glm::vec3& dest)
		:origin(ori), dir(glm::normalize(dest)) {}
	
	inline glm::vec3 GetOrigin() const { return origin; }
	inline glm::vec3 GetDir() const { return dir; }
	inline glm::vec3 PointAtParameter(float t) const { return origin + t * dir; }
	
	inline void moveOriginBy(float t) { origin += t * dir; }
	
	glm::vec3 test = 1.0f * glm::vec3(0.1, 0.1, 0.1);
};
