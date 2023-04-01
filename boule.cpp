#include "boule.h"

//namespace
using namespace sf;
using namespace std;


Boule::Boule() {
	weight = 1;  
	size = 10;
	xpos = 0;
	ypos = 0;
	xposLast = xpos;
	yposLast = ypos;
	xacc = 0;
	yacc = 0;
	index = -1;
}

void Boule::Init(int ind, int x, int y, double s) {
	size = s;
	index = ind;
	xpos = x;
	ypos = y;
	xposLast = xpos;
	yposLast = ypos;
}

void Boule::Update() {
}

void Boule::AddSpeed(double speedx, double speedy, double dt) {
	xposLast = xpos - speedx*dt;
	yposLast = ypos - speedy*dt;
}
