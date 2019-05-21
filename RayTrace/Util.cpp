#include "Util.h"

bool Util::IsFormatPPM(const char* filename)
{
	std::regex r("(.+)\.ppm$");
	return std::regex_match(filename, r);
}



void Util::TestImage_Gradient(const char* filename, int imageWidth, int imageHeight)
{
	if (!Util::IsFormatPPM(filename)) {
		std::cout << "ERROR: " << filename << " is in wrong format. Acceptable file format: <filename>.ppm" << std::endl;
		return;
	}

	std::ofstream myfile;
	myfile.open(filename);

	if (myfile.is_open()) {
		
		std::cout << "Writing into file " << std::string(filename) << std::endl;

		myfile << "P3\n" << imageWidth << " " << imageHeight << std::endl;
		myfile << "255" << std::endl;

		for (int j = imageHeight - 1; j >= 0; j--) {
			for (int i = 0; i < imageWidth; i++) {

				glm::vec3 v3(float(i) / float(imageWidth), float(j) / float(imageHeight), 0.2f);

				int ir = int(255.99 * v3.r);
				int ig = int(255.99 * v3.g);
				int ib = int(255.99 * v3.b);

				myfile << ir << " " << ig << " " << ib << std::endl;
			}
		}

		myfile.close();

		std::cout << "Finished writing file." << std::endl;
	}
	else {
		std::cout << "ERROR: File " << std::string(filename) << " unable to be written to." << std::endl;
		return;
	}
}



void Util::TestImage_BlueSky(const char* filename, int imageWidth, int imageHeight)
{
	if (!Util::IsFormatPPM(filename)) {
		std::cout << "ERROR: " << filename << " is in wrong format. Acceptable file format: <filename>.ppm" << std::endl;
		return;
	}

	std::ofstream myfile;
	myfile.open(filename);

	if (myfile.is_open()) {

		std::cout << "Writing into file " << std::string(filename) << std::endl;

		myfile << "P3\n" << imageWidth << " " << imageHeight << std::endl;
		myfile << "255" << std::endl;

		glm::vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
		glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
		glm::vec3 vertical(0.0f, 2.0f, 0.0f);
		glm::vec3 origin;

		for (int j = imageHeight - 1; j >= 0; j--) {
			for (int i = 0; i < imageWidth; i++) {
				float u = float(i) / float(imageWidth);
				float v = float(j) / float(imageHeight);
				
				Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
				glm::vec3 colour = Util::BlueSky_Color(r);

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
		std::cout << "ERROR: File " << std::string(filename) << " unable to be written to." << std::endl;
		return;
	}
}
