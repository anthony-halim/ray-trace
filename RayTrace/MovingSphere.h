#pragma once

#include "Polygon.h"

class MovingSphere : public Polygon {

	glm::vec3 centreZero;
	glm::vec3 centreFinal;
	float radius;
	Material* pMat;

	//Movement information
	float movementDuration;
	float tInitial;
	float tCurrentFrame;

public:

	MovingSphere()
		: centreZero(glm::vec3(0.0f, 0.0f, 0.0f)), centreFinal(glm::vec3(0.0f, 0.0f, 0.0f)), 
			radius(0.0f), pMat(nullptr), movementDuration(0.0f), tInitial(0.0f), tCurrentFrame(0.0f) {}

	MovingSphere(const glm::vec3& cent0, const glm::vec3& cent1, float r, Material* mat, float dur, float t0)
		: centreZero(cent0), centreFinal(cent1), radius(r), pMat(mat), movementDuration(dur), tInitial(t0), tCurrentFrame(t0) {}

	glm::vec3 GetCentreAtInstant(float t) const {

		float intP;
		glm::vec3 movementDir = centreFinal - centreZero;
		return centreZero + (std::modf((t / movementDuration), &intP) * movementDir);
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
				GetUV(record);
				return true;
			}

			temp = (-b + sqrt(discr)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - newCentre) / radius;
				record.pMat_ptr = pMat;
				GetUV(record);
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
