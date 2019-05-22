#include "Util.h"

bool Util::IsFormatPPM(const char* filename)
{
	std::regex r("(.+)\.ppm$");
	return std::regex_match(filename, r);
}


