#include "Util.h"
#include "PolygonList.h"
#include "Sphere.h"

#define IMAGE_WIDTH 200
#define IMAGE_HEIGHT 100
#define T_MIN 1.0e-4f
#define T_MAX 1.0e4f

#define FILENAME "output.ppm"

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

		glm::vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
		glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
		glm::vec3 vertical(0.0f, 2.0f, 0.0f);
		glm::vec3 origin;

		// Scene Initialisation
		Polygon* list[2];
		list[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
		list[1] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);

		Polygon * world = new PolygonList(list, 2);

		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				float u = float(i) / float(nx);
				float v = float(j) / float(ny);

				Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

				glm::vec3 p = r.PointAtParameter(2.0f);
				glm::vec3 colour = GetColour(r, world);

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

