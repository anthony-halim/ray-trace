#include "Util.h"
#include "PolygonList.h"

#include "LambertianDiffuse.h"
#include "Metal.h"
#include "Dielectric.h"

#include "Sphere.h"
#include "MovingSphere.h"

#include "BVH_Node.h"
#include "Camera.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 256

#define FILENAME "output.ppm"

#define T_MIN 1.0e-4f
#define T_MAX 1.0e4f
#define ANTI_ALIASING_SAMPLE_NUM 50
#define MAX_RECURSION_LEVEL 50

#define CAMERA_APERTURE 0.01f

// Forward Declaration
static void SimulateAndWritePPM();
static void InitialiseScene();
static glm::vec3 GetColour(const Ray& r, Polygon* world, int recursionLevel = 0);



// Global variables
bool g_IsAntiAliasingActivated = false;
BVH_Node* g_WorldRoot = nullptr;



int main() {
	InitialiseScene();
	SimulateAndWritePPM();
}



static glm::vec3 GetColour(const Ray& r, Polygon* world, int recursionLevel) {
	
	SHitRecord record;

	if (world->IsHit(r, T_MIN, T_MAX, record)) {

		Ray scattered;
		glm::vec3 attenuation;

		if (recursionLevel < MAX_RECURSION_LEVEL && record.pMat_ptr->IsScattered(r, record, attenuation, scattered)) {
			return attenuation * GetColour(scattered, world, recursionLevel + 1);
		}
		
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else {
		float t = 0.5f * (r.GetDir().y + 1.0f);
		return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
	}
}



static void InitialiseScene() {

	std::cout << "Initialising scene..." << std::endl;

	Polygon ** list = new Polygon*[4];

	list[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new LambertianDiffuse(glm::vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new LambertianDiffuse(glm::vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f));
	list[3] = new MovingSphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f), 0.25, 1.0, 0.0);
	
	std::cout << "Initialising BVH Tree..." << std::endl;

	g_WorldRoot = new BVH_Node(list, 4, 0.0f, 0.0f);

	std::cout << "Finished setting up BVH Tree." << std::endl;
	std::cout << "Finished initialising scene." << std::endl;
}



static void SimulateAndWritePPM() {
	
	if (!Util::IsFormatPPM(FILENAME)) {
		std::cout << "ERROR: " << FILENAME << " is in wrong format. Acceptable file format: <filename>.ppm" << std::endl;
		return;
	}

	std::ofstream myfile;
	myfile.open(FILENAME);

	int nx = IMAGE_WIDTH;
	int ny = IMAGE_HEIGHT;
	int ns = ANTI_ALIASING_SAMPLE_NUM;

	glm::vec3 lookFrom(0.0f, 0.0f, 0.0f);
	glm::vec3 lookAt(0.0f, 0.0f, -1.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	float focusDist = glm::length(lookFrom - lookAt);
	float aperture = CAMERA_APERTURE;

	Camera mainCamera(lookFrom, lookAt, up,
		90.0, ((float)nx) / ny,
		aperture, focusDist);

	mainCamera.SetShutterTiming(1.0f, 0.0f);

	if (myfile.is_open()) {

		std::cout << "Writing into file: " << std::string(FILENAME) << std::endl;

		myfile << "P3\n" << nx << " " << ny << std::endl;
		myfile << "255" << std::endl;

		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				
				// Anti aliasing by shooting multiple ray per pixel and averaging the result
				glm::vec3 colour(0.0f, 0.0f, 0.0f);
				
				for (int s = 0; s < ns; s++) {
					float u = float(i + ((float)rand() / RAND_MAX)) / float(nx);
					float v = float(j + ((float)rand() / RAND_MAX)) / float(ny);
					Ray r = mainCamera.GetRay(u, v);
					glm::vec3 p = r.PointAtParameter(2.0f);
					colour += GetColour(r, g_WorldRoot, 0);
				}
				colour /= float(ns);

				// Gamma correction
				colour = glm::vec3(pow(colour.x, 0.4), pow(colour.y, 0.4), pow(colour.z, 0.4));

				int ir = int(255.99 * colour.r);
				int ig = int(255.99 * colour.g);
				int ib = int(255.99 * colour.b);

				// Write into file
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

