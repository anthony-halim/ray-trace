#pragma once

#include "Ray.h"

class Camera {
private:

	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 origin;

public:
	Camera() {
		lowerLeftCorner = glm::vec3(-2.0f, -1.0f, -1.0f);
		horizontal = glm::vec3(4.0f, 0.0f, 0.0f);
		vertical = glm::vec3(0.0f, 2.0f, 0.0f);
		origin = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Ray GetRay(float u, float v) {
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
	}

};