#pragma once

#include "Texture.h"
#include <glm/trigonometric.hpp>

class CheckeredTexture : public Texture {
private:
	Texture* odd;
	Texture* even;
	float patternScale;
public:
	CheckeredTexture(Texture * t0, Texture *t1, float scale)
		: odd(t0), even(t1), patternScale(scale) {}

	~CheckeredTexture() {
		delete odd;
		delete even;
	}

	virtual glm::vec3 Value(float u, float v, const glm::vec3& p) const override {
		float sines = glm::sin(patternScale * p.x) * glm::sin(patternScale * p.y) * glm::sin(patternScale * p.z);
		bool isOdd = sines < 0;

		return (float)isOdd * odd->Value(u, v, p) + (float)!isOdd * even->Value(u, v, p);
	}
};
