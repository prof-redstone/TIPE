#ifndef FUNC_H
#define FUNC_H
#include <iostream>
#include <vector>
#include <cmath>
#include "boule.h"


using namespace std;

void loadFont();
int PRNG(int seed, int index);
double rnd(int seed, int i);
float lerp(float a, float b, float t);
float perlin_noise(int seed, float x);
vector <Boule> shuffle(vector <Boule> tab, int n, int seed);

#endif