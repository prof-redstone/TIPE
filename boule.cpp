#include "boule.h"

//namespace
using namespace sf;
using namespace std;


Boule::Boule() {
	weight = 1;  
	size = 10;
	xpos = 0;
	ypos = 0;
	xspeed = 0;
	yspeed = 0;
	index = -1;
}

void Boule::Init(int ind, int x, int y, double s) {
	size = s;
	index = ind;
	xpos = x;
	ypos = y;
}

void Boule::Update() {
	

}
