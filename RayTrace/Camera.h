#pragma once

#include "Ray.h"
#include <glm/trigonometric.hpp>

class Camera {
private:

	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 origin;
	glm::vec3 u, v, w;
	float lensRadius;

public:
	Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect, float aperture, float focusDist) {
		
		lensRadius = aperture / 2;

		float theta = glm::radians(vfov);
		float halfHeight = glm::tan(theta / 2.0f);
		float halfWidth = aspect * halfHeight;
		
		origin = lookfrom;
		w = glm::normalize(lookfrom - lookat);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);

		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	Ray GetRay(float s, float t) {
		glm::vec3 rd = lensRadius * Util::GetRandomVec3_unitDisk();
		glm::vec3 offset = u * rd.x + v * rd.y;
		return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}

};