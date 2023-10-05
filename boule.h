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
	double xposLast; //pas de variable vitesse, on la calcule en faisant la difference entre pos et poslast (pour des raison pratique)
	double yposLast;
	double xacc;
	double yacc;
	int index;
	bool tire;

	Boule();
	void Init(int ind, int x, int y, double s);
	void ResetSpeed();
	void AddSpeed(double speedx, double speedy, double dt);
};



#endif