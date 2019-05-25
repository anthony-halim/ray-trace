#pragma once

#include "Polygon.h"

class PolygonList : public Polygon {

public:

	Polygon** plygonList;
	int listSize;

	PolygonList() { plygonList = nullptr; listSize = 0; }
	PolygonList(Polygon** l, int n) { plygonList = l; listSize = n; }

	~PolygonList() {
		for (int i = 0; i < listSize; i++) {
			delete plygonList[i];
		}
	}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const {
		
		SHitRecord tempRec;
		bool hitAnything = false;
		float closestSoFar = t_max;

		int size = listSize;

		for (int i = 0; i < listSize; i++) {
			int count = i;
			bool hit = plygonList[i]->IsHit(r, t_min, closestSoFar, tempRec);
			if (plygonList[i]->IsHit(r, t_min, closestSoFar, tempRec)) {
				hitAnything = true;
				closestSoFar = tempRec.t;
				record = tempRec;
			}
		}
		return hitAnything;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		if (listSize < 1) return false;
		AABB tempBox;
		bool firstTrue = plygonList[0]->BoundingBox(t0, t1, tempBox);

		if (!firstTrue) return false;

		box = tempBox;

		for (int i = 1; i < listSize; i++) {
			if (plygonList[i]->BoundingBox(t0, t1, tempBox)) {
				box = Util::SurroundingBox(box, tempBox);
			}
			else { return false; }
		}

		return true;
	}
};

