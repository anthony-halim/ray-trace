#pragma once

#include "Material.h"

class DiffuseLight : public Material {
private:
	Texture* emit;
public:
	DiffuseLight(Texture* e)
		: emit(e) {}

	virtual glm::vec3 Emits(float u, float v, const glm::vec3& p) const override {
		return emit->Value(u, v, p);
	}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
		return false;
	}
};