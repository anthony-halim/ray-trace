#pragma once

#include "Material.h"

class Util;

class Dielectric : public Material {
private:
	float refrIndex;
public:
	Dielectric(float ri)
		: refrIndex(ri) {
		refrIndex = refrIndex < 1 ? 1.0f : refrIndex;
	}

	virtual bool IsScattered(const Ray& r_in, const SHitRecord& rec, glm::vec3& attenuation, Ray& scattered) const override {
		glm::vec3 outwardNormal;
		glm::vec3 refracted;
		float correctedRefrIndex;
		float reflectProb;
		float cosine;
		
		glm::vec3 reflected = Util::ReflectAboutN(r_in.GetDir(), rec.normal);

		attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

		if (glm::dot(r_in.GetDir(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			correctedRefrIndex = refrIndex;
			cosine = refrIndex * glm::dot(r_in.GetDir(), rec.normal);
		}
		else {
			outwardNormal = rec.normal;
			correctedRefrIndex = 1.0f / refrIndex;
			cosine = -1 * glm::dot(r_in.GetDir(), rec.normal);
		}

		bool isRefractable = Util::Refract(r_in.GetDir(), outwardNormal, correctedRefrIndex, refracted);
		reflectProb = isRefractable * Util::FresnelApproximation(cosine, refrIndex) + !isRefractable * 1.0f;

		bool isReflected = ((float)rand() / RAND_MAX) < reflectProb;
		scattered = Ray(rec.p, (float)isReflected * reflected + (float)!isReflected * refracted);
		
		return true;
	}
};