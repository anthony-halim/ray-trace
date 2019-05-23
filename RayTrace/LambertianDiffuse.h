#pragma once

#include "Material.h"

class Util;

class LambertianDiffuse : public Material {

private:

	glm::vec3 albedo;

public:

	LambertianDiffuse(const glm::vec3& alb)
		:albedo(alb) {}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& record, glm::vec3& attenuation, Ray& scattered) const override {
		glm::vec3 target = record.p + record.normal + Util::GetRandomVec3_unitSphere();
		scattered = Ray(record.p, target - record.p);
		attenuation = albedo;
		return true;
	}
};