#pragma once

#include "Texture.h"
#include "Perlin.h"

class NoiseTexture : public Texture {
private:
	Perlin noise;
	float scale;
	unsigned int mode;
public:
	NoiseTexture()
		: scale(1.0f), mode(0) {}
	NoiseTexture(float sc, unsigned int m = 0)
		: scale(sc), mode(m) {}
	virtual glm::vec3 Value(float u, float v, const glm::vec3& p) const override {
		switch (mode) {
		case 0:
			return glm::vec3(1.0f, 1.0f, 1.0f) * noise.Noise(scale * p);
		case 1:
		default:
			return glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f * (1 + glm::sin(scale * p.z + scale * noise.Turbulance(p)));
		}
	}
	void ChangeMode(unsigned int i) { mode = i & 1; }

};