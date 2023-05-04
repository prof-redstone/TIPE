
#include <iostream> //pour ecrire dans la console.
#include <random>
#include <vector>
#include <cmath>
#include <string>


using namespace std;

double rnd(int seed, int i) {
	std::mt19937 gen(seed);
	std::uniform_int_distribution<int> dist(0, 999999);
	gen.seed(seed + i);
	double res = ((double)dist(gen)) / 1000000;
	return res;
}