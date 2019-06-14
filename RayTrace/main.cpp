#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Util.h"
#include "BVH_Node.h"
#include "Camera.h"

#include "DiffuseLight.h"
#include "LambertianDiffuse.h"
#include "Metal.h"
#include "Dielectric.h"

#include "ImageTexture.h"
#include "ConstantTexture.h"
#include "CheckeredTexture.h"
#include "NoiseTexture.h"

#include "FlipNormals.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "MovingSphere.h"
#include "Box.h"
#include "Translate.h"
#include "RotateY.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 256

#define FILENAME "output.ppm"

#define T_MIN 1.0e-4f
#define T_MAX 1.0e4f
#define ANTI_ALIASING_SAMPLE_NUM 100
#define MAX_RECURSION_LEVEL 100

#define CAMERA_APERTURE 0.01f

// Forward Declaration
static Texture* ReadImageTexture(const char* filename);
static void SimulateAndWritePPM();
static void InitialiseScene();
static void CleanUpMemory();
static glm::vec3 RayTrace(const Ray& r, Polygon* world, int recursionLevel = 0);

static Polygon** SimpleScene(int& listSize);
static Polygon** RandomScene(int& listSize);
static Polygon** PerlinScene(int& listSize);
static Polygon** TestScene(int& listSize);
static Polygon** CornellBoxScene(int& listSize);

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
		glm::vec3 emitted = record.pMat_ptr->Emits(record.u, record.v, record.p);
		if (recursionLevel < MAX_RECURSION_LEVEL && record.pMat_ptr->IsScattered(r, record, attenuation, scattered)) {
			return emitted + attenuation * RayTrace(scattered, world, recursionLevel + 1);
		}
		return emitted;
	}
	else {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}



static ImageTexture* CreateImageTexture(const char* filename) {
	int nx, ny, nn;
	unsigned char* texData = stbi_load(filename, &nx, &ny, &nn, 0);

	if (texData == NULL)
		std::cout << "Unable to load texture: " << filename << std::endl;

	return new ImageTexture(texData, nx, ny);
}



static void InitialiseScene() {

	std::cout << "Initialising scene..." << std::endl;

	int listSize;
	Polygon** sceneObjList = CornellBoxScene(listSize);

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

	glm::vec3 lookFrom(278.f, 278.f, -800.f);
	glm::vec3 lookAt(278.f, 278.f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	//glm::vec3 lookFrom(13.f, 2.f, 3.f);
	//glm::vec3 lookAt(0.f, 0.f, 0.f);
	//glm::vec3 up(0.0f, 1.0f, 0.0f);
	float vfov = 40.0f;	
	float focusDist = 10.0f;
	float aperture = CAMERA_APERTURE;

	Camera mainCamera(lookFrom, lookAt, up,
		vfov, ((float)nx) / ny,
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

static Polygon** SimpleScene(int& listSize) {

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
	Polygon** list = new Polygon * [n];

	Texture* perTex = new NoiseTexture(2.5f, 1);

	list[0] = new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, new LambertianDiffuse(perTex));
	list[1] = new Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2, new LambertianDiffuse(perTex));

	listSize = n;
	return list;
}

static Polygon** TestScene(int& listSize) {
	int n = 3;
	Polygon** list = new Polygon * [n];

	Texture* perTex = new NoiseTexture(4.0f, 1);
	
	int nx, ny, nn;
	unsigned char* texData = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
	Material* earthMat = new LambertianDiffuse(new ImageTexture(texData, nx, ny));

	list[0] = new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, new LambertianDiffuse(perTex));
	list[1] = new Sphere(glm::vec3(0.0f, 2.0f, 0.0f), 2, earthMat);
	list[2] = new Rectangle_XY(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(glm::vec3(4.0f, 4.0f, 4.0f))));

	listSize = n;
	return list;
}

static Polygon** CornellBoxScene(int& listSize) {

	int n = 8;
	Polygon** list = new Polygon * [n];

	Material* red = new LambertianDiffuse(new ConstantTexture(glm::vec3(0.65f, 0.05f, 0.05f)));
	Material* white = new LambertianDiffuse(new ConstantTexture(glm::vec3(0.73f, 0.73f, 0.73f)));
	Material* green = new LambertianDiffuse(new ConstantTexture(glm::vec3(0.12f, 0.45f, 0.15f)));
	Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(4.0f, 4.0f, 4.0f)));
	
	list[0] = new FlipNormals(new Rectangle_YZ(0, 555, 0, 555, 555, green));
	list[1] = new Rectangle_YZ(0, 555, 0, 555, 0, red);
	list[2] = new Rectangle_XZ(213, 343, 227, 332, 554, light);
	list[3] = new FlipNormals(new Rectangle_XZ(0, 555, 0, 555, 555, white));
	list[4] = new Rectangle_XZ(0, 555, 0, 555, 0, white);
	list[5] = new FlipNormals(new Rectangle_XY(0, 555, 0, 555, 555, white));
	
	list[6] = new Translate(
		new RotateY(
			new Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 165.0f, 165.0f), white), 
			-18.0f),
		glm::vec3(130.0f, 0.0f, 65.0f));

	list[7] = new Translate(
		new RotateY(
			new Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 330.0f, 165.0f), white), 
			15.0f),
		glm::vec3(265.0f, 0.0f, 295.0f));

	listSize = n;
	return list;
}