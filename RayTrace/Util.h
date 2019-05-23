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

	// Returns a ray where x, y, z are all within (-1, 1)
	static glm::vec3 GetRandomVec3_unitSphere();
};

