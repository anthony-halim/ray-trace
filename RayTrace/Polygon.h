#pragma once

#include "Ray.h"

class Material;

struct SHitRecord {
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	Material* pMat_ptr;
};

class Polygon {
public:
	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const = 0;
};