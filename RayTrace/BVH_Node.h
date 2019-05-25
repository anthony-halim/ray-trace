#pragma once

#include "Util.h"
#include "Polygon.h"

class BVH_Node : public Polygon {
public:

	BVH_Node() {}
	BVH_Node(Polygon** list, int n, float time0, float time1);
	
	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const;
	virtual bool BoundingBox(float t0, float t1, AABB& _box) const;

public:
	Polygon* left;
	Polygon* right;
	AABB box;

};