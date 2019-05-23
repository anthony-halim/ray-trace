#pragma once

#include "Material.h"

class Util;

class Metal : public Material {

private:

	glm::vec3 albedo;
	float roughness;

public:

	Metal(const glm::vec3& alb, float r)
		:albedo(alb), roughness(r) {}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
		glm::vec3 reflected = Util::ReflectAboutN(r_in.GetDir(), rec.normal);
		scattered = Ray(rec.p, reflected + roughness * Util::GetRandomVec3_unitSphere());
		attenuation = albedo;
		return glm::dot(scattered.GetDir(), rec.normal) > 0;
	}
};