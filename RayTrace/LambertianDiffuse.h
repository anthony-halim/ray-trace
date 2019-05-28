#pragma once

#include "Material.h"

class Util;

class LambertianDiffuse : public Material {

private:

	Texture * albedo;

public:

	LambertianDiffuse(Texture * alb)
		:albedo(alb) {}

	~LambertianDiffuse() {
		delete albedo;
	}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& record, glm::vec3& attenuation, Ray& scattered) const override {
		glm::vec3 target = record.p + record.normal + Util::GetRandomVec3_unitSphere();
		scattered = Ray(record.p, target - record.p, r_in.GetTime());
		attenuation = albedo->Value(record.u, record.v, record.p);
		return true;
	}
};