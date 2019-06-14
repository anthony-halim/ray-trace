#pragma once

#include "Material.h"
#include "Util.h"

class Isotropic : public Material {
private:
	Texture* albedo;
public:
	Isotropic(Texture* a)
		: albedo(a) {}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
		scattered = Ray(rec.p, Util::GetRandomVec3_unitSphere());
		attenuation = albedo->Value(rec.u, rec.v, rec.p);
		return true;
	}
};