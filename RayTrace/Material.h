#pragma once

#include "Polygon.h"

class Material {
public:
	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

