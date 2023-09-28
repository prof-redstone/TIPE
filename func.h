#ifndef FUNC_H
#define FUNC_H
#include <iostream>
#include <vector>
#include <cmath>


using namespace std;

int PRNG(int seed, int index);
double rnd(int seed, int i);
vector <int> shuffle(vector <int> tab, int n, int seed);

#endif