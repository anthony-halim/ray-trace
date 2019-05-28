#pragma once

#include "Polygon.h"

#define M_PI 3.14159265358979323846264338327950288f

class Sphere : public Polygon {

private:

	glm::vec3 centre;
	float radius;
	Material* pMat;

private:

	void GetUV(SHitRecord& record) const {
		glm::vec3 correctedP = (record.p - centre) / radius;

		float phi = glm::atan(correctedP.z, correctedP.x);
		float theta = glm::asin(correctedP.y);

		record.u = 1 - (phi - M_PI) / (2 * M_PI);
		record.v = (theta + M_PI / 2) / M_PI;
	}

public:

	Sphere()
		: centre(glm::vec3(0.0f, 0.0f, 0.0f)), radius(0.0f), pMat(nullptr) {}

	Sphere(const glm::vec3& cent, float r, Material * mat)
		: centre(cent), radius(r), pMat(mat) {}

	bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {

		glm::vec3 oc = r.GetOrigin() - centre;
		float a = 1.0f;
		float b = 2.0f * glm::dot(oc, r.GetDir());
		float c = glm::dot(oc, oc) - radius * radius;
		float discr = b * b - 4 * a * c;

		if (discr > 0) {
			float temp = (-b - sqrt(discr)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - centre) / radius;
				record.pMat_ptr = pMat;
				GetUV(record);
				return true;
			}

			temp = (-b + sqrt(discr)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - centre) / radius;
				record.pMat_ptr = pMat;
				GetUV(record);
				return true;
			}
		}

		return false;
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const override {
		box = AABB(centre - glm::vec3(radius, radius, radius), centre + glm::vec3(radius, radius, radius));
		return true;
	}

};