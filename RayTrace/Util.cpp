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

glm::vec3 Util::GetRandomVec3_unitDisk()
{
	glm::vec3 p;
	
	float x = (float)rand() / RAND_MAX;
	float y = (float)rand() / RAND_MAX;
	float z = 0.0f;

	p = 2.0f * glm::vec3(x, y, z) - glm::vec3(1.0f, 1.0f, 0.0f);
	return glm::normalize(p);
}


glm::vec3 Util::ReflectAboutN(const glm::vec3& v, const glm::vec3& n) {
	glm::vec3 normalisedN = glm::normalize(n);
	return v - 2 * glm::dot(v, normalisedN) * normalisedN;
}

float Util::FresnelApproximation(float cosine, float refrIndex)
{
	float rZero = (1 - refrIndex) / (1 + refrIndex);
	float rZeroSq = rZero * rZero;
	return rZeroSq + (1 - rZeroSq) * pow((1 - cosine), 5);
}

bool Util::Refract(const glm::vec3& v, const glm::vec3& n, float refrIndex, glm::vec3& refracted)
{
	glm::vec3 normalisedV = glm::normalize(v);
	float dt = glm::dot(normalisedV, n);

	float discr = 1.0f - refrIndex * refrIndex * (1.0f - dt * dt);

	if (discr > 0) {
		refracted = refrIndex * (normalisedV - n * dt) - n * sqrt(discr);
		return true;
	}
	return false;
}

