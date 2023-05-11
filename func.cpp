
#include <iostream> //pour ecrire dans la console.
#include <random>
#include <vector>
#include <cmath>
#include <string>


using namespace std;

int PRNG(int seed, int index)
{
    // our initial starting seed is 5323
    static unsigned int nSeed = seed*5323 + index*8647;

    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 32767
    return nSeed % 32767;
}

double rnd(int seed, int index) {
    int res = PRNG(seed, index) % 10000;
    return ((double)res) / 10000;
}