#include "AABB.h"

bool AABB::IsHit(const Ray& r, float t_min, float t_max) const
{
	for (int i = 0; i < 3; i++) {

		float invD = 1.0f / r.GetDir()[i];

		float t0 = (min[i] - r.GetOrigin()[i]) * invD;
		float t1 = (max[i] - r.GetOrigin()[i]) * invD;

		if (invD < 0.0f) {
			float temp = t1;
			t1 = t0;
			t0 = temp;
		}

		t_min = (t0 > t_min) * t0 + !(t0 > t_min) * t_min;
		t_max = (t1 < t_max) * t1 + !(t1 < t_max) * t_max;

		if (t_max <= t_min) return false;
	}

	return true;
}
