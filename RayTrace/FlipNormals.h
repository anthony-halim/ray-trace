#pragma once

#include "Polygon.h"

class FlipNormals : public Polygon {
public:
	
	FlipNormals(Polygon* p)
		: objPtr(p) {}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		if (objPtr->IsHit(r, t_min, t_max, record)) {
			record.normal = -record.normal;
			return true;
		}
		return false;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		return objPtr->BoundingBox(t0, t1, box);
	}

private:
	Polygon* objPtr;
};