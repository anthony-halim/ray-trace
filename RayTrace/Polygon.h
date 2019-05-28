#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;

struct SHitRecord {
	float t;
	float u;
	float v;
	glm::vec3 p;
	glm::vec3 normal;
	Material* pMat_ptr;
};

class Polygon {

public:
	virtual void GetUV(SHitRecord& record) const {
		record.u = 0;
		record.v = 0;
	}
	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const = 0;
};