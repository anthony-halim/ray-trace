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

		Polygon * one = plygonList[0];
		Polygon * two = plygonList[1];

		bool res1 = one->IsHit(r, t_min, closestSoFar, tempRec);
		bool res2 = two->IsHit(r, t_min, closestSoFar, tempRec);


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
};

