#include "Util.h"
#include "PolygonList.h"
#include "Sphere.h"
#include "Camera.h"


#define IMAGE_WIDTH 200
#define IMAGE_HEIGHT 100

#define FILENAME "output.ppm"

#define T_MIN 1.0e-4f
#define T_MAX 1.0e4f
#define ANTI_ALIASING_SAMPLE_NUM 50

// Forward Declaration
static void SimulateAndWritePPM();
static glm::vec3 GetColour(const Ray& r, Polygon* world);


int main() {
	SimulateAndWritePPM();
}

static glm::vec3 GetColour(const Ray& r, Polygon* world) {
	SHitRecord record;

	if (world->IsHit(r, T_MIN, T_MAX, record)) {
		return 0.5f * glm::vec3(record.normal.x + 1, record.normal.y + 1, record.normal.z + 1);
	}
	else {
		float t = 0.5f * (r.GetDir().y + 1.0f);
		return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}

static void SimulateAndWritePPM() {
	
	int nx = IMAGE_WIDTH;
	int ny = IMAGE_HEIGHT;
	int ns = ANTI_ALIASING_SAMPLE_NUM;

	if (!Util::IsFormatPPM(FILENAME)) {
		std::cout << "ERROR: " << FILENAME << " is in wrong format. Acceptable file format: <filename>.ppm" << std::endl;
		return;
	}

	std::ofstream myfile;
	myfile.open(FILENAME);

	if (myfile.is_open()) {

		std::cout << "Writing into file " << std::string(FILENAME) << std::endl;

		myfile << "P3\n" << nx << " " << ny << std::endl;
		myfile << "255" << std::endl;

		Camera mainCamera;

		// Scene Initialisation
		Polygon* list[2];
		list[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
		list[1] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);

		Polygon * world = new PolygonList(list, 2);

		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				glm::vec3 colour(0.0f, 0.0f, 0.0f);

				// Anti aliasing by shooting multiple ray per pixel and averaging the result
				for (int s = 0; s < ns; s++) {
					float u = float(i + ((float)rand() / RAND_MAX)) / float(nx);
					float v = float(j + ((float)rand() / RAND_MAX)) / float(ny);
					Ray r = mainCamera.GetRay(u, v);
					glm::vec3 p = r.PointAtParameter(2.0f);
					colour += GetColour(r, world);
				}
				colour /= float(ns);

				int ir = int(255.99 * colour.r);
				int ig = int(255.99 * colour.g);
				int ib = int(255.99 * colour.b);

				myfile << ir << " " << ig << " " << ib << std::endl;
			}
		}
		myfile.close();

		std::cout << "Finished writing file." << std::endl;
	}
	else {
		std::cout << "ERROR: File " << std::string(FILENAME) << " unable to be written to." << std::endl;
		return;
	}
}

