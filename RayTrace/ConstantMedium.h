#pragma once

#include "Polygon.h"
#include "Texture.h"
#include "Isotropic.h"

class ConstantMedium : public Polygon {

private:

	Polygon* boundary;
	float density;
	Material* phaseFunc;

public:
	
	ConstantMedium(Polygon* bound, float dens, Texture* tex)
		: boundary(bound), density(dens) {
		phaseFunc = new Isotropic(tex);
	}

	virtual bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {
		const float epsilon = 0.0001f;
		SHitRecord recOne, recTwo;

		bool db = ((float)rand() / RAND_MAX) < epsilon;
		bool isHitRecOne = boundary->IsHit(r, INT_MIN, INT_MAX, recOne);
		bool isHitRecTwo = isHitRecOne ? boundary->IsHit(r, recOne.t + epsilon, INT_MAX, recTwo) : false;

		if (isHitRecOne && isHitRecTwo) {
			recOne.t = glm::max(recOne.t, t_min);
			recTwo.t = glm::min(recTwo.t, t_max);
			if (recOne.t >= recTwo.t) return false;

			recOne.t *= (recOne.t >= 0); // Clamp to [0, recOne.t]
			float distInsideBoundary = (recTwo.t - recOne.t);
			float hitDist = -(1 / density) * log((float)rand() / RAND_MAX);
			
			if (hitDist < distInsideBoundary) {
				record.t = recOne.t + hitDist / glm::length(r.GetDir());
				record.p = r.PointAtParameter(record.t);
				record.normal = glm::vec3(1.0f, 0.0f, 0.0f); // arbitrary
				record.pMat_ptr = phaseFunc;
				return true;
			}
		}
		return false;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		return boundary->BoundingBox(t0, t1, box);
	}
	
};