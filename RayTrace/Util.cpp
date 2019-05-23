#include "Util.h"

bool Util::IsFormatPPM(const char* filename)
{
	std::regex r("(.+)\.ppm$");
	return std::regex_match(filename, r);
}

glm::vec3 Util::GetRandomVec3_unitSphere() {

	glm::vec3 p;

	float x = (float)rand() / RAND_MAX;
	float y = (float)rand() / RAND_MAX;
	float z = (float)rand() / RAND_MAX;

	p = 2.0f * glm::vec3(x, y, z) - glm::vec3(1, 1, 1);

	return glm::normalize(p);
}

glm::vec3 Util::ReflectAboutN(const glm::vec3& v, const glm::vec3& n) {
	glm::vec3 normalisedN = glm::normalize(n);
	return v - 2 * glm::dot(v, normalisedN) * normalisedN;
}

