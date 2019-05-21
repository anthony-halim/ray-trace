#pragma once

#include <iostream>
#include <fstream>
#include <regex>

#include "Ray.h"

namespace Util {

	bool IsFormatPPM(const char* filename);

	void TestImage_Gradient(const char* filename, int imageWidth, int imageHeight);
	void TestImage_BlueSky(const char* filename, int imageWidth, int imageHeight);


	glm::vec3 BlueSky_Color(const Ray& r) {
		float t = 0.5f * (r.GetDir().y + 1.0f);
		return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5, 0.7, 1.0);
	}
};

