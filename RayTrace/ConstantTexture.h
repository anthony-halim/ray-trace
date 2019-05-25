#pragma once

#include "Texture.h"

class ConstantTexture : public Texture {

private:

	glm::vec3 colour;

public:
	
	ConstantTexture()
		: colour(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	
	ConstantTexture(const glm::vec3& c)
		: colour(c) {}
	
	virtual glm::vec3 Value(float u, float v, const glm::vec3& p) const override {
		return colour;
	}
};