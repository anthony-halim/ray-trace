#pragma once

#include "Polygon.h"
#include "Texture.h"

class Material {
public:
	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
};

