#pragma once

#include "Polygon.h"

class Sphere : public Polygon {

private:

	glm::vec3 centre;
	float radius;

public:
	Sphere()
		: centre(glm::vec3(0.0f, 0.0f, 0.0f)), radius(0.0f) {}

	Sphere(const glm::vec3& cent, float r)
		: centre(cent), radius(r) {}

	bool IsHit(const Ray& r, float t_min, float t_max, SHitRecord& record) const override {

		glm::vec3 oc = r.GetOrigin() - centre;
		float a = 1.0f;
		float b = 2.0f * glm::dot(oc, r.GetDir());
		float c = 1 - radius * radius;
		float discr = b * b - 4 * a * c;

		if (discr > 0) {
			float temp = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - centre) / radius;
				return true;
			}

			temp = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);

			if (temp < t_max && temp > t_min) {
				record.t = temp;
				record.p = r.PointAtParameter(temp);
				record.normal = (record.p - centre) / radius;
				return true;
			}
		}

		return false;
	}
};