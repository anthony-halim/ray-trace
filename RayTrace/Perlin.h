#pragma once

#include <glm/vec3.hpp>

class Perlin {
private:
	
	static glm::vec3* ranVec;
	static int* permX;
	static int* permY;
	static int* permZ;

private:
	
	static void Permutate(int* p, int n) {
		for (int i = n - 1; i > 0; i--) {
			int target = int((float)rand() / RAND_MAX * (i + 1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
		return;
	}

	inline static float PerlinInterp(glm::vec3 c[8], float u, float v, float w) {
		
		float uu = u * u * (3 - 2 * u);
		float vv = v * v * (3 - 2 * v);
		float ww = w * w * (3 - 2 * w);
		
		float acc = 0;
		
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					glm::vec3 weightV(u - i, v - j, w - k);
					acc += (i * uu + (1 - i) * (1 - uu)) *
						(j * vv + (1 - j) * (1 - vv)) *
						(k * ww + (1 - k) * (1 - ww)) *
						glm::dot(c[i * 4 + j * 2 + k], weightV);
				}
			}
		}
		return acc;
	}

public:
	
	static float Noise(const glm::vec3& p) {
		float u = p.x - floor(p.x);
		float v = p.y - floor(p.y);
		float w = p.z - floor(p.z);

		int i = floor(p.x);
		int j = floor(p.y);
		int k = floor(p.z);

		glm::vec3 c[8];

		for (int di = 0; di < 2; di++) {
			for (int dj = 0; dj < 2; dj++) {
				for (int dk = 0; dk < 2; dk++) {
					int index = permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255];
					c[di * 4 + dj * 2 + dk] = ranVec[index];
				}
			}
		}

		return PerlinInterp(c, u, v, w);
	}

	static glm::vec3* PerlinGenerate() {
		glm::vec3* p = new glm::vec3[256];
		float x, y, z;
		for (int i = 0; i < 256; i++) {
			x = (-1.0f + 2 * (float)rand() / RAND_MAX);
			y = (-1.0f + 2 * (float)rand() / RAND_MAX);
			z = (-1.0f + 2 * (float)rand() / RAND_MAX);
			p[i] = glm::normalize(glm::vec3(x, y, z));
		}
		return p;
	}

	static int* PerlinGenerate_Permutate() {
		int* p = new int[256];
		for (int i = 0; i < 256; i++) {
			p[i] = i;
		}
		Permutate(p, 256);
		return p;
	}

	static float Turbulance(const glm::vec3& p, int depth = 5) {
		float acc = 0.0f;
		glm::vec3 temp = p;
		float weight = 1.0f;
		for (int i = 0; i < depth; i++) {
			acc += weight * Noise(temp);
			weight *= 0.5f;
			temp *= 2;
		}
		return std::abs(acc);
	}
};

glm::vec3 * Perlin::ranVec = Perlin::PerlinGenerate();
int* Perlin::permX = Perlin::PerlinGenerate_Permutate();
int* Perlin::permY = Perlin::PerlinGenerate_Permutate();
int* Perlin::permZ = Perlin::PerlinGenerate_Permutate();
