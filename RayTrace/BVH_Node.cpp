#include "BVH_Node.h"

int BoxCompareX(const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Polygon* ah = *(Polygon * *)a;
	Polygon* bh = *(Polygon * *)b;

	if (!ah->BoundingBox(0, 0, boxLeft) || !bh->BoundingBox(0, 0, boxRight)) {
		std::cout << "ERROR: There is no bounding box in BVH_Node constructor" << std::endl;
	}

	bool result = boxLeft.GetMin().x - boxRight.GetMin().x < 0.0f;
	return result * -1 + !result * 1;
}

int BoxCompareY(const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Polygon* ah = *(Polygon * *)a;
	Polygon* bh = *(Polygon * *)b;

	if (!ah->BoundingBox(0, 0, boxLeft) || !bh->BoundingBox(0, 0, boxRight)) {
		std::cout << "ERROR: There is no bounding box in BVH_Node constructor" << std::endl;
	}

	bool result = boxLeft.GetMin().y - boxRight.GetMin().y < 0.0f;
	return result * -1 + !result * 1;
}

int BoxCompareZ(const void* a, const void* b)
{
	AABB boxLeft, boxRight;
	Polygon* ah = *(Polygon * *)a;
	Polygon* bh = *(Polygon * *)b;

	if (!ah->BoundingBox(0, 0, boxLeft) || !bh->BoundingBox(0, 0, boxRight)) {
		std::cout << "ERROR: There is no bounding box in BVH_Node constructor" << std::endl;
	}

	bool result = boxLeft.GetMin().z - boxRight.GetMin().z < 0.0f;
	return result * -1 + !result * 1;
}

BVH_Node::BVH_Node(Polygon** list, int n, float time0, float time1)
{
	int axis = rand() % 3;

	switch (axis) {
	case 0:
		qsort(list, n, sizeof(Polygon*), BoxCompareX);
		break;
	case 1:
		qsort(list, n, sizeof(Polygon*), BoxCompareY);
		break;
	case 2:
		qsort(list, n, sizeof(Polygon*), BoxCompareZ);
	}

	if (n == 1) { 
		left = right = list[0]; 
	}
	else if (n == 2) { 
		left = list[0]; right = list[1]; 
	}
	else {
		left = new BVH_Node(list, n / 2, time0, time1);
		right = new BVH_Node(list + (n / 2), n - (n / 2), time0, time1);
	}

	AABB boxLeft, boxRight;

	if (!left->BoundingBox(time0, time1, boxLeft) || !right->BoundingBox(time0, time1, boxRight)) {
		std::cout << "ERROR: There is no bounding box in BVH_Node constructor" << std::endl;
	}

	box = Util::SurroundingBox(boxLeft, boxRight);
}

bool BVH_Node::IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const
{
	if (box.IsHit(r, t_min, t_max)) {
		SHitRecord leftRec, rightRec;
		bool hitLeft = left->IsHit(r, t_min, t_max, leftRec);
		bool hitRight = right->IsHit(r, t_min, t_max, rightRec);

		if (hitLeft && hitRight) {
			if (leftRec.t < rightRec.t) record = leftRec;
			else record = rightRec;
			return true;
		}
		else if (hitLeft) {
			record = leftRec;
			return true;
		}
		else if (hitRight) {
			record = rightRec;
			return true;
		}
		else { return false; }
	}
	return false;
}

bool BVH_Node::BoundingBox(float t0, float t1, AABB& _box) const
{
	_box = box;
	return true;
}



