#ifndef BRASSEUR_H
#define BRASSEUR_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>  //to initialize random

class Brasseur {
public:

	//boule parameter
	double xpos;
	double ypos;
	int centerX;
	int centerY;
	double size;
	double angle;
	double rayon;
	double speedAng;

	Brasseur();
	void Init(double a, double r, double s, double sr, int cX, int cY);
	void Update();
};






#endif