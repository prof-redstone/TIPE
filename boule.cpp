#include "boule.h"

//namespace
using namespace sf;
using namespace std;


Boule::Boule() {
	double weight = 1;  
	double size = 1;
	double xpos = 0;
	double ypos = 0;
	double xspeed = 0;
	double yspeed = 0;

	int index = -1;
}

void Boule::Init(int ind, int x, int y) {
	size = weight;
	index = ind;
	xpos = x;
	ypos = y;
}

void Boule::Update() {

}
