#pragma once

#include "Polygon.h"

class Translate : public Polygon {
private:
	Polygon* pObj;
	glm::vec3 offset;
public:
	Translate(Polygon* p, const glm::vec3 displacement) 
		: pObj(p), offset(displacement) {}
	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		Ray moved_r(r.GetOrigin() - offset, r.GetDir(), r.GetTime());
		if (pObj->IsHit(moved_r, t_min, t_max, record)) {
			record.p += offset;
			return true;
		}
		return false;
	}
	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		if (pObj->BoundingBox(t0, t1, box)) {
			box = AABB(box.GetMin() + offset, box.GetMax() + offset);
			return true;
		}
		return false;
	}

};
