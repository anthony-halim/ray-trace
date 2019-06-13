#pragma once

#include "Texture.h"

class ImageTexture : public Texture {
private:
	unsigned char* data;
	int nx;
	int ny;
public:
	ImageTexture(unsigned char * pixels, int numPixX, int numPixY) 
		: data(pixels), nx(numPixX), ny(numPixY) {}

	~ImageTexture() { delete data; }

	virtual glm::vec3 Value(float u, float v, const glm::vec3& p) const override {
		int i = int(u * nx);
		int j = int((1 - v) * ny - 0.001);
		
		// Clamp to [0, n*)
		i = (i < 0) * 0 + !(i < 0) * i;
		j = (j < 0) * 0 + !(j < 0) * j;

		i = (i > nx - 1) * (nx - 1) + !(i > nx - 1) * i;
		j = (j > ny - 1) * (ny - 1) + !(j > ny - 1) * j;

		float r = int(data[3 * i + 3 * nx * j]) / 255.0f;
		float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0f;
		float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0f;
		
		return glm::vec3(r, g, b);
	}
};