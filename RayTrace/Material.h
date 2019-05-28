#pragma once

#include "Polygon.h"
#include "Texture.h"

class Material {
public:
	virtual glm::vec3 Emits(float u, float v, const glm::vec3& p) const {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

