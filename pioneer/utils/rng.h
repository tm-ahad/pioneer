#pragma once
#include <random>

using namespace std;

class RandomNumberGenerator
{
public:
    RandomNumberGenerator();
    char generateByteNumber();

private:
    uniform_int_distribution<int> dist;
    mt19937 rng;
};
