#include "brasseur.h"

//namespace
using namespace sf;
using namespace std;


Brasseur::Brasseur() {
	size = 10;
	xpos = 0;
	ypos = 0;
	angle = 0;
	rayon = 100;
	speedAng = 0.1;
	centerX = 0;
	centerY = 0;
}

void Brasseur::Init(double a, double r, double s, double sr, int cX, int cY) {
	angle = a;
	rayon = r;
	size = s;
	speedAng = sr;
	centerX = cX;
	centerY = cY;
	xpos = centerX + cos(angle) * rayon;
	ypos = centerY + sin(angle) * rayon;
}

void Brasseur::Update() {
	angle += speedAng; //deltaTime = 1, change speedRot
	xpos = centerX + cos(angle) * rayon;
	ypos = centerY + sin(angle) * rayon;
}