#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// Orthogonal Normal Bases 

class ONB {
private:
	glm::vec3 axis[3];
public:
	ONB(const glm::vec3& n) {
		axis[2] = glm::normalize(n);
		glm::vec3 a;
		a = (float)(axis[2][0] > 0.9f) * glm::vec3(0.0f, 1.0f, 0.0f) + (float)(axis[2][0] <= 0.9f) * glm::vec3(1.0f, 0.0f, 0.0f);
		axis[1] = glm::normalize(glm::cross(axis[2], a));
		axis[0] = glm::cross(axis[2], axis[1]);
	}

	inline glm::vec3 operator[] (int i) const { return axis[i]; }
	glm::vec3 u() const { return axis[0]; }
	glm::vec3 v() const { return axis[1]; }
	glm::vec3 w() const { return axis[2]; }
	glm::vec3 local(float a, float b, float c) const { return a * axis[0] + b * axis[1] + c * axis[2]; }
	glm::vec3 local(const glm::vec3& a) const { return a[0] * axis[0] + a[1] * axis[1] + a[2] * axis[2]; }
};