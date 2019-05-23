#pragma once

#include "Ray.h"
#include <glm/trigonometric.hpp>

class Camera {
private:

	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 origin;

public:
	Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect) {
		glm::vec3 u, v, w;

		float theta = glm::radians(vfov);
		float halfHeight = glm::tan(theta / 2.0f);
		float halfWidth = aspect * halfHeight;
		
		origin = lookfrom;
		w = glm::normalize(lookfrom - lookat);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);

		lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}

	Ray GetRay(float u, float v) {
		return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
	}

};