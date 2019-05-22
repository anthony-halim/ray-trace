#pragma once

#include "Polygon.h"

class Sphere : public Polygon {

private:

	glm::vec3 origin;
	float radius;

public:
	Sphere()
		: origin(glm::vec3(0.0f, 0.0f, 0.0f)), radius(0.0f) {}

	Sphere(const glm::vec3& ori, float r)
		: origin(ori), radius(r) {}

	bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {

		glm::vec3 oc = r.GetOrigin() - origin;
		float a = glm::dot(r.GetDir(), r.GetDir());
		float b = 2.0f * glm::dot(oc, r.GetDir());
		float c = glm::dot(oc, oc) - radius * radius;
		float discr = b * b - 4 * a * c;

		if (discr > 0) {
			float temp = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - origin) / radius;
				return true;
			}

			temp = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - origin) / radius;
				return true;
			}
		}

		return false;
	}
};