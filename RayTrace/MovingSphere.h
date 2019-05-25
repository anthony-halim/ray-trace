#pragma once

#include "Polygon.h"

class MovingSphere : public Polygon {

	glm::vec3 centre;
	float radius;
	Material* pMat;

	//Movement information
	float movementRange;
	float movementDuration;
	float tInitial;

public:

	MovingSphere()
		: centre(glm::vec3(0.0f, 0.0f, 0.0f)), radius(0.0f), pMat(nullptr),
		movementRange(0.0f), movementDuration(0.0f), tInitial(0.0f) {}

	MovingSphere(const glm::vec3& cent, float r, Material* mat,
		float range, float dur, float t0)
		: centre(cent), radius(r), pMat(mat),
		movementRange(range), movementDuration(dur), tInitial(t0) {}

	glm::vec3 GetCentreAtInstant(float t) const {
		float intP;
		return centre + (std::modf((t / movementDuration), &intP) * movementRange);
	}

	bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {

		glm::vec3 newCentre = GetCentreAtInstant(r.GetTime());

		glm::vec3 oc = r.GetOrigin() - newCentre;
		float a = 1.0f;
		float b = 2.0f * glm::dot(oc, r.GetDir());
		float c = glm::dot(oc, oc) - radius * radius;
		float discr = b * b - 4 * a * c;

		if (discr > 0) {
			float temp = (-b - sqrt(discr)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - newCentre) / radius;
				record.pMat_ptr = pMat;
				return true;
			}

			temp = (-b + sqrt(discr)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - newCentre) / radius;
				record.pMat_ptr = pMat;
				return true;
			}
		}

		return false;
	}

	bool BoundingBox(float t0, float t1, AABB& box) const override {
		glm::vec3 centreAtStart = GetCentreAtInstant(tInitial);
		glm::vec3 centreAtEnd = GetCentreAtInstant(tInitial + movementDuration);
		
		AABB boxTimeStart = AABB(centreAtStart - glm::vec3(radius, radius, radius), centreAtStart + glm::vec3(radius, radius, radius));
		AABB boxTimeEnd = AABB(centreAtEnd - glm::vec3(radius, radius, radius), centreAtEnd + glm::vec3(radius, radius, radius));
		
		box = Util::SurroundingBox(boxTimeStart, boxTimeEnd);
		
		return true;
	}

};
