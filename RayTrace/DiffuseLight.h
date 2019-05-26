#pragma once

#include "Material.h"

class DiffuseLight : public Material {
private:
	Texture* emit;
public:
	DiffuseLight(Texture*a)
		: emit(a) {}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
		return false;
	}

	virtual glm::vec3 Emission(float u, float v, glm::vec3& p) const override {
		return emit->Value(u, v, p);
	}
};
