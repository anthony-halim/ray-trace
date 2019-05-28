#pragma once

#include "Material.h"

class Util;

class Metal : public Material {

private:

	Texture * albedo;
	float roughness;

public:

	Metal(Texture * alb, float r)
		:albedo(alb), roughness(r) {}

	~Metal() {
		delete albedo;
	}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
		glm::vec3 reflected = Util::ReflectAboutN(r_in.GetDir(), rec.normal);
		scattered = Ray(rec.p, reflected + roughness * Util::GetRandomVec3_unitSphere(), r_in.GetTime());
		attenuation = albedo->Value(rec.u, rec.v, rec.p);
		return glm::dot(scattered.GetDir(), rec.normal) > 0;
	}
};