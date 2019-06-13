#pragma once

#include "Polygon.h"
#include "Rectangle.h"
#include "FlipNormals.h"

class Box : public Polygon {
private:
	glm::vec3 pmin, pmax;
	Polygon** listPtr;
public:
	Box(const glm::vec3& p0, const glm::vec3& p1, Material* mat)
		: pmin(p0), pmax(p1) {
		listPtr = new Polygon * [6];
		listPtr[0] = new Rectangle_XY(p0.x, p1.x, p0.y, p1.y, p1.z, mat);
		listPtr[1] = new FlipNormals(new Rectangle_XY(p0.x, p1.x, p0.y, p1.y, p0.z, mat));
		listPtr[2] = new Rectangle_XZ(p0.x, p1.x, p0.z, p1.z, p1.y, mat);
		listPtr[3] = new FlipNormals(new Rectangle_XZ(p0.x, p1.x, p0.z, p1.z, p0.y, mat));
		listPtr[4] = new Rectangle_YZ(p0.y, p1.y, p0.z, p1.z, p1.x, mat);
		listPtr[5] = new FlipNormals(new Rectangle_YZ(p0.y, p1.y, p0.z, p1.z, p0.x, mat));
	}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		SHitRecord tempRec;
		bool hitAnything = false;
		float closestSoFar = t_max;

		for (int i = 0; i < 6; i++) {
			int count = i;
			bool hit = listPtr[i]->IsHit(r, t_min, closestSoFar, tempRec);
			if (listPtr[i]->IsHit(r, t_min, closestSoFar, tempRec)) {
				hitAnything = true;
				closestSoFar = tempRec.t;
				record = tempRec;
			}
		}
		return hitAnything;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		box = AABB(pmin, pmax);
		return true;
	}
};