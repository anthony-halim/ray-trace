#include "Util.h"
#include "BVH_Node.h"
#include "Camera.h"

#include "LambertianDiffuse.h"
#include "Metal.h"
#include "Dielectric.h"

#include "ConstantTexture.h"
#include "CheckeredTexture.h"
#include "NoiseTexture.h"

#include "Sphere.h"
#include "MovingSphere.h"

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
static glm::vec3 RayTrace(const Ray& r, Polygon* world, int recursionLevel = 0);

static Polygon** TestScene(int& listSize);
static Polygon** RandomScene(int& listSize);
static Polygon** PerlinScene(int& listSize);

// Global variables
bool g_IsAntiAliasingActivated = false;
BVH_Node* g_WorldRoot = nullptr;



int main() {
	InitialiseScene();
	SimulateAndWritePPM();
}



static glm::vec3 RayTrace(const Ray& r, Polygon* world, int recursionLevel) {
	
	SHitRecord record;

	if (world->IsHit(r, T_MIN, T_MAX, record)) {

		Ray scattered;
		glm::vec3 attenuation;

		if (recursionLevel < MAX_RECURSION_LEVEL && record.pMat_ptr->IsScattered(r, record, attenuation, scattered)) {
			return attenuation * RayTrace(scattered, world, recursionLevel + 1);
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

	int listSize;
	Polygon** sceneObjList = PerlinScene(listSize);

	std::cout << "Number of objects in the scene: " << listSize << std::endl;
	std::cout << "Initialising BVH Tree..." << std::endl;

	g_WorldRoot = new BVH_Node(sceneObjList, listSize, 0.0f, 0.0f);
	
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

	glm::vec3 lookFrom(13.0f, 2.0f, 3.0f);
	glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	float focusDist = 10.0f;
	float aperture = CAMERA_APERTURE;

	Camera mainCamera(lookFrom, lookAt, up,
		20.0, ((float)nx) / ny,
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
					colour += RayTrace(r, g_WorldRoot, 0);
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

static Polygon** TestScene(int& listSize) {

	int n = 4;
	Polygon** list = new Polygon * [n];
	
	list[0] = new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f,
		new LambertianDiffuse(new ConstantTexture(glm::vec3(0.8f, 0.3f, 0.3f))));

	list[1] = new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f,
		new LambertianDiffuse(new ConstantTexture(glm::vec3(0.8f, 0.8f, 0.0f))));

	list[2] = new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f,
		new Metal(new CheckeredTexture(
			new ConstantTexture(glm::vec3(0.8f, 0.6f, 0.2f)),
			new ConstantTexture(glm::vec3(0.3f, 0.8f, 0.2f)),
			10), 0.3f));

	list[3] = new MovingSphere(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(-1.0f, 0.3f, -1.0f), 0.5f,
		new Dielectric(2.417f), 1.0, 0.0);

	listSize = n;
	return list;
}

static Polygon** RandomScene(int& listSize) {

	int n = 400;

	Polygon** list = new Polygon * [n + 1];
	
	Texture* checker = new CheckeredTexture(
		new ConstantTexture(glm::vec3(0.2f, 0.3f, 0.1f)),
		new ConstantTexture(glm::vec3(0.9f, 0.9f, 0.9f)), 10);

	list[0] = new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
		new LambertianDiffuse(checker));

	int i = 1;
	for (int a = -5; a < 5; a++) {
		for (int b = -5; b < 5; b++) {
			
			float matChoice = (float)rand() / RAND_MAX;
			glm::vec3 centre(a + 0.9f * (float)rand() / RAND_MAX, 0.2f, b + 0.9f * (float)rand() / RAND_MAX);
			
			if (glm::length(centre - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
				if (matChoice < 0.8f) { // Diffuse
					
					Texture* randConstTex = new ConstantTexture(
						glm::vec3(((float)rand()) / RAND_MAX, 
								  ((float)rand()) / RAND_MAX, 
								  ((float)rand()) / RAND_MAX));
					
					list[i++] = new MovingSphere(
						centre, centre + glm::vec3(0, 0.5f * ((float)rand()) / RAND_MAX, 0), 0.2f,
						new LambertianDiffuse(randConstTex), 1.0f, 0.0f);
				
				}
				else if (matChoice < 0.95f) { // Metal

					Texture* randMetalConstTex = new ConstantTexture(
						glm::vec3(0.5f * (1.0f + ((float)rand()) / RAND_MAX),
								  0.5f * (1.0f + ((float)rand()) / RAND_MAX),
								  0.5f * (1.0f + ((float)rand()) / RAND_MAX)));

					list[i++] = new Sphere(centre, 0.2f,
						new Metal(randMetalConstTex, 0.5f * (float)rand() / RAND_MAX));
				}
				else { // Glass
					list[i++] = new Sphere(centre, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(2.7f));
	list[i++] = new Sphere(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, new LambertianDiffuse(
		new ConstantTexture(glm::vec3(0.4f, 0.2f, 0.1f))));
	list[i++] = new Sphere(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(
		new ConstantTexture(glm::vec3(0.7f, 0.6f, 0.5f)), 0.1f));

	listSize = i;
	return list;
}

static Polygon** PerlinScene(int& listSize) {
	
	int n = 2;
	Polygon** list = new Polygon * [2];

	Texture* perTex = new NoiseTexture(2.5f, 1);

	list[0] = new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, new LambertianDiffuse(perTex));
	list[1] = new Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2, new LambertianDiffuse(perTex));

	listSize = n;
	return list;
}
