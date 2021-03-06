#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Ray {

private:
	
	glm::vec3 origin;
	glm::vec3 dir;
	float time;

public:
	
	Ray()
		:origin(glm::vec3(0, 0, 0)), dir(glm::vec3(0, 0, 0)), time(0.0f) {}

	Ray(const glm::vec3& ori, const glm::vec3& dest, float t = 0.0f)
		:origin(ori), dir(glm::normalize(dest)), time(t) {}
	
	inline glm::vec3 GetOrigin() const { return origin; }
	inline glm::vec3 GetDir() const { return dir; }
	inline float GetTime() const { return time; }
	inline glm::vec3 PointAtParameter(float t) const { return origin + t * dir; }
	
	inline void moveOriginBy(float t) { origin += t * dir; }
};
