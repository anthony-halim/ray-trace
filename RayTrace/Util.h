#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Util {

public:
	static bool IsFormatPPM(const char* filename);
	
	// Returns a ray where x, y, z are all within (-1, 1)
	static glm::vec3 getRandomVec3_unitSphere() {
		
		glm::vec3 p;

		float x = (float)rand() / RAND_MAX;
		float y = (float)rand() / RAND_MAX;
		float z = (float)rand() / RAND_MAX;

		p = 2.0f * glm::vec3(x, y, z) - glm::vec3(1, 1, 1);

		return glm::normalize(p);
	}
};

