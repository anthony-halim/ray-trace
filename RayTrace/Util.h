#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Util {

public:

	// Checks whether filename is in .ppm format
	static bool IsFormatPPM(const char* filename);

	static bool Refract(const glm::vec3& v, const glm::vec3& n, float refrIndex, glm::vec3& refracted);

	// Returns a ray where x, y, z are all within (-1, 1)
	static glm::vec3 GetRandomVec3_unitSphere();

	static glm::vec3 ReflectAboutN(const glm::vec3& v, const glm::vec3& n);

	// Based on polynomial approximation by Christophe Schlick
	static float FresnelApproximation(float cosine, float refrIndex) {
		float rZero = (1 - refrIndex) / (1 + refrIndex);
		float rZeroSq = rZero * rZero;
		return rZeroSq + (1 - rZeroSq) * pow((1 - cosine), 5);
	}

};

