#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "AABB.h"

class Util {

public:

	// Checks whether filename is in .ppm format
	static bool IsFormatPPM(const char* filename);

	static bool Refract(const glm::vec3& v, const glm::vec3& n, float refrIndex, glm::vec3& refracted);

	// Returns a ray where x, y, z are all within (-1, 1)
	static glm::vec3 GetRandomVec3_unitSphere();

	// Returns a ray where x, y are all within (-1, 1) and is 0
	static glm::vec3 GetRandomVec3_unitDisk();

	static glm::vec3 ReflectAboutN(const glm::vec3& v, const glm::vec3& n);

	// Based on polynomial approximation by Christophe Schlick
	static float FresnelApproximation(float cosine, float refrIndex);

	inline static float rt_min(float a, float b) {
		return (a < b) * a + !(a < b) * b;
	}

	inline static float rt_max(float a, float b) {
		return (a > b)* a + !(a > b) * b;
	}

	static AABB SurroundingBox(const AABB& box0, const AABB& box1);

};

