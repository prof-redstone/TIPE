#include "boule.h"

//namespace
using namespace sf;
using namespace std;


Boule::Boule() {
	weight = 1;  
	size = 5;
	xpos = 0;
	ypos = 0;
	xposLast = xpos;
	yposLast = ypos;
	xspeed = 0;
	yspeed = 0;
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
