#include "simulation.h"

#include "boule.h"

using namespace sf;
using namespace std;



Simulation::Simulation() {

	win_width = 1000; //default value, use updateWindow to change this value
	win_height = 1000;

	//graphic part
	image.create(win_width, win_height, Color::Cyan);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture);


	//Boule part
	nbBoule = 1;
	deltaTime = 0.01;
	vector < Boule > boules;
}

void Simulation::Init() {
	boules.clear();
	for (int i = 0; i < nbBoule; i++) {
		boules.push_back(Boule());
		boules[i].Init(i, 50, 50, 10);
		boules[i].Update();
	}
}

void Simulation::Print() {
	cout << to_string(win_height) << endl;
}

void Simulation::Update() {
	//ordre exection des etapes :
	/*
		1)gravity
		2)check collision
		3)faire les contrainte de collision, bordure piece en mouvement dans le future
		4)update la position
	*/
	ApplyForce();
	ResolveConstraint();

}

void Simulation::ApplyForce(){
	//application de la gravité et autre force potentielle
	for (int i = 0; i < nbBoule; i++) {
		//Gravity Force 
		//am=f donc a=f/m
		double gravity = 1000;
		double yf = gravity * boules[i].weight; //foce sur l'axe x
		double yacc = yf / boules[i].weight; //acceleration sur l'axe x
		boules[i].yspeed += yacc * deltaTime;
		boules[i].ypos += boules[i].yspeed * deltaTime;
	}
}
void Simulation::ResolveCollision(){
	cout << "ici" << endl;
	//check collision entre boulle -------a tester -------
	const double repCoef = 0.75;
	for (int i = 0; i < nbBoule; i++) { //onjet 1
		for (int j = i + 1; j < nbBoule; j++) { // objet 2 different de objet 1, pour faire une paire
			double distx = boules[i].xpos - boules[j].xpos;
			double disty = boules[i].ypos - boules[j].ypos;
			double dist2 = distx * distx + disty * disty;
			double distmin = boules[i].size + boules[j].size;

			//wheck overlapping
			if (dist2 < distmin * distmin) {
				float dist1 = sqrt(dist2);
				//nb sous-step :
				const double nx = distx / dist1;
				const double ny = disty / dist1;
				const double massRatio1 = boules[i].size / (boules[i].size + boules[j].size);
				const double massRatio2 = boules[j].size / (boules[i].size + boules[j].size);
				const double delta = 0.5 * repCoef * (dist1 - distmin);
				boules[i].xpos -= nx * (massRatio2 * delta);
				boules[i].ypos -= ny * (massRatio2 * delta);
				boules[j].xpos += nx * (massRatio1 * delta);
				boules[j].ypos += ny * (massRatio1 * delta);
			}
		}
	}
}
void Simulation::ResolveConstraint(){
	//application des contrainte de mouvement :
	//zone de mouvement : cercle de taille de la fenettre centre (win_width = radius)
	double zoneRadius = win_width/2;
	for (int i = 0; i < nbBoule; i++) {
		double distx = (win_width/2) -  boules[i].xpos;
		double disty = (win_height/2) -  boules[i].ypos;
		double dist = sqrt(distx*distx + disty*disty);
		if (dist > zoneRadius- boules[i].size)
		{
			const double nx = distx / dist;
			const double ny = disty / dist;
			boules[i].xpos = (win_width/2) - nx*(zoneRadius - boules[i].size);
			boules[i].ypos = (win_width/2) - ny*(zoneRadius - boules[i].size);
		}
		
	}
}

void Simulation::UpdateSpeed(){
	
}

void Simulation::Show() {
	//clear screen
	Color black;
	black.g = 0;
	black.r = 0;
	black.b = 0;
	for (int i = 0; i < win_width; i++) {
		for (int j = 0; j < win_height; j++) {
			image.setPixel(i, j, black);
		}
	}


	DrawBoule();
}

void Simulation::DrawBoule() {
	for (int i = 0; i < nbBoule; i++) { //indice de la boule
		int x = (int) boules[i].xpos;
		int y = (int) boules[i].ypos;
		int r = (int) boules[i].size;
		for (int j = x - r; j < x + r; j++) {
			for (int k = y - r; k < y + r; k++) {
				if ((j >= 0 && j < win_width) && (k >= 0 && k < win_height)) {
					double dist = sqrt(((double) pow(j - x, 2)) + ((double) pow(k - y, 2)));
					if (dist <= (double) r) {
						Color red;
						red.r = 255;
						image.setPixel(j, k, red);
					}
				}
			}
		}
	}
}

void Simulation::UpdateWindow(sf::RenderWindow & win) {
	win_width = win.getSize().x;
	win_height = win.getSize().y;
	image.create(win_width, win_height, Color::Black);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture, true);
}


void Simulation::Render(sf::RenderWindow & win) {

	//creation de la texture par defaut
	//image dans la texture
	texture.create(win_width, win_height);
	texture.update(image);

	//texture dans le sprite
	sprite.setTexture(texture, false);

	win.draw(sprite);

}