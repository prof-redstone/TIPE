#ifndef BOULE_H
#define BOULE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>  //to initialize random

class Boule {
public:

	//boule parameter
	double weight;
	double size;
	double xpos;
	double ypos;
	double xspeed;
	double yspeed;
	int index;

	Boule();
	void Init(int ind, int x, int y);
	void Update();
};



#endif