#include "stdafx.h"
#include "core/RNG.h"

//constructor
RNG::RNG() : m_generator(std::random_device()()) {}

//public functions
int RNG::generateRandomNum(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(m_generator);
}



