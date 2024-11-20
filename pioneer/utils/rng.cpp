#include "rng.h"

using namespace std;

RandomNumberGenerator::RandomNumberGenerator() 
	: rng(random_device{}()), dist(0, 255) {}

char RandomNumberGenerator::generateByteNumber()
{
	return static_cast<char>(dist(rng));
}
