#include "simulation.h"

#include "boule.h"
#include "brasseur.h"

using namespace sf;
using namespace std;


//pour initialiser l'objet simulation
Simulation::Simulation() {

	win_width = 1000; //default value, use updateWindow to change this value
	win_height = 1000;

	//graphic part
	image.create(win_width, win_height, Color::Cyan);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture);


	//Boule part
	deltaTime = 0.005;//par defaut (a changer dans le Init)
	nbFrameSkip = 3;
	time = 0;
	nbBoule = 0; //changera dans Init
	vector < Boule > boules;

	nbBrasseur = 0; //changera dans Init
	vector < Brasseur > brasseurs;
}

//pour initialiser les parametre, et remetre a 0 les boules
void Simulation::Init(double dt, int nbB) {
	deltaTime = dt;

	//pour generer les boules au début de la simulation
	nbBoule = nbB;
	boules.clear();
	for (int i = 0; i < nbBoule; i++) {
		boules.push_back(Boule());
		boules[i].Init(i, 100 + 10*i, 100, 10);
	}

	//pour generer les brasseur, taille position et rayon
	nbBrasseur = 5;
	double sizeBrasseur = 30; // rayon du brasseur
	double speedBrasseur = 0.002; //la vitesse de rotation des brasseurs
	for (int i = 0; i < nbBrasseur; i++){
		brasseurs.push_back(Brasseur()) ;
		double angsec = (2 * 3.1415) / nbBrasseur;
		brasseurs[i].Init(i * angsec, (win_width/2)-sizeBrasseur, sizeBrasseur, speedBrasseur, win_width / 2, win_height / 2);
	}

}


void Simulation::UpdateWindow(sf::RenderWindow& win) { //appele une fois au début du prog pour initialiser la taille de la fenettre, ou en cours pour redimmentionner la fenettre
	cout << "UpdateWindow" << endl;
	win_width = win.getSize().x;
	win_height = win.getSize().y;
	Color LayerColor = Color(0, 0, 0, 0);
	image.create(win_width, win_height, LayerColor);
	BGimg.create(win_width, win_height, LayerColor);
	sprite.setTexture(texture, true);

	//create the circle background 
	Color bg = Color(0, 0, 0);
	Color circle = Color(50, 50, 50);
	for (int i = 0; i < win_width; i++)
	{
		for (int j = 0; j < win_height; j++)
		{
			double dist = sqrt(((double)pow(i - win_width / 2, 2)) + ((double)pow(j - win_height / 2, 2)));
			if (dist <= (double)(win_width / 2)) {
				BGimg.setPixel(i, j, circle);
			}
		}
	}
}

//debug, fait apparaitre des balles pendant la simulation, à retirer lors des experiences finals
void Simulation::AddBall() {
	if (time % 20 == 0) {
		boules.push_back(Boule());
		boules[nbBoule].Init(nbBoule, 100, 200, 10);
		boules[nbBoule].AddSpeed(0, 1000, deltaTime);
		nbBoule++;
	}
}

//main function of program, call at each frame
void Simulation::Update() {
	UpdateBrasseur();
	//pour les balles :
	/*	ordre exection des etapes :
		1)gravity
		2)check collision
		3)faire les contrainte de collision, bordure, brasseur
		4)update la position
	*/
	ApplyForce();
	ResolveConstraint();
	ResolveCollision();
	UpdateBall();
	time++;
}

//appliquer les différente force du systeme sur les balles
void Simulation::ApplyForce(){
	//application de la gravité et autre force potentielle
	for (int i = 0; i < nbBoule; i++) {
		//Gravity Force 
		//am=f donc a=f/m
		double gravity = 1000;
		double yf = gravity * boules[i].weight; //foce sur l'axe x
		boules[i].yacc = yf / boules[i].weight; //acceleration sur l'axe x
	}
}


//ce qui fait que ça fonctionne, pour gerer les collisions avec les différentes balles
void Simulation::ResolveCollision(){
	//check collision entre les boulles
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
	double zoneRadius = win_width/2 ;//rayon de la zone libre
	for (int i = 0; i < nbBoule; i++) {
		double distx = (win_width/2) -  boules[i].xpos;//distance de la boule au centre 
		double disty = (win_height/2) -  boules[i].ypos;
		double dist = sqrt(distx*distx + disty*disty);
		if (dist > zoneRadius - boules[i].size){
			const double nx = distx / dist;
			const double ny = disty / dist;
			boules[i].xpos = (win_width/2) - nx*(zoneRadius - boules[i].size);
			boules[i].ypos = (win_height/2) - ny*(zoneRadius - boules[i].size);
		}
		
	}

	for (int i = 0; i < nbBoule; i++){
		for (int j = 0; j < nbBrasseur; j++){
			double distx = brasseurs[j].xpos - boules[i].xpos;//distance entre les 2 cercles 
			double disty = brasseurs[j].ypos - boules[i].ypos;
			double dist = sqrt(distx * distx + disty * disty);
			if (dist < brasseurs[j].size + boules[i].size)
			{
				const double nx = distx / dist;
				const double ny = disty / dist;
				boules[i].xpos = brasseurs[j].xpos - nx * (brasseurs[j].size + boules[i].size);
				boules[i].ypos = brasseurs[j].ypos - ny * (brasseurs[j].size + boules[i].size);
			}
		}
	}
}

void Simulation::UpdateBall(){
	for (int i = 0; i < nbBoule; i++){	
		double distx = boules[i].xpos - boules[i].xposLast;
		double disty = boules[i].ypos - boules[i].yposLast;
		boules[i].xposLast = boules[i].xpos;
		boules[i].yposLast = boules[i].ypos;
		boules[i].xpos = boules[i].xpos + distx + boules[i].xacc * deltaTime * deltaTime;
		boules[i].ypos = boules[i].ypos + disty + boules[i].yacc * deltaTime * deltaTime;


	}
}

void Simulation::UpdateBrasseur() {
	for (int i = 0; i < nbBrasseur; i++)
	{
		brasseurs[i].Update();
	}
}


void Simulation::DrawBoule() { //pour afficher les boules une par une sur l'image
	for (int i = 0; i < nbBoule; i++) { //indice de la boule
		int x = (int) boules[i].xpos;
		int y = (int) boules[i].ypos;
		int r = (int) boules[i].size;
		Color col = HSLtoRGB(((double)boules[i].index) / 50.0, 0.1, 1, 1);
		for (int j = x - r; j < x + r; j++) {
			for (int k = y - r; k < y + r; k++) {
				if ((j >= 0 && j < win_width) && (k >= 0 && k < win_height)) {
					double dist = sqrt(((double) pow(j - x, 2)) + ((double) pow(k - y, 2)));
					if (dist <= (double) r) {
						image.setPixel(j, k, col);
					}
				}
			}
		}
	}
}

void Simulation::DrawBrasseur() {
	for (int i = 0; i < nbBrasseur; i++) { //indice de la boule
		int x = (int)brasseurs[i].xpos;
		int y = (int)brasseurs[i].ypos;
		int r = (int)brasseurs[i].size;
		Color col = Color(250, 250, 250);
		for (int j = x - r; j < x + r; j++) {
			for (int k = y - r; k < y + r; k++) {
				if ((j >= 0 && j < win_width) && (k >= 0 && k < win_height)) {
					double dist = sqrt(((double)pow(j - x, 2)) + ((double)pow(k - y, 2)));
					if (dist <= (double)r) {
						image.setPixel(j, k, col);
					}
				}
			}
		}
	}
}



void Simulation::Render(sf::RenderWindow & win) { //appele a chaque frame pour afficher le gb et les boules.
	if (time % (nbFrameSkip+1) == 0) {//pour economiser les performances 

		win.clear();
		//clear screen
		Color black = Color(0,0,0,0);
		for (int i = 0; i < win_width; i++) {
			for (int j = 0; j < win_height; j++) {
				image.setPixel(i, j, black);
			}
		}

		DrawBoule();
		DrawBrasseur();

		//creation de la texture par defaut
		//image dans la texture
		texture.create(win_width, win_height);
		texture.update(image);
		BGtext.create(win_width, win_height);
		BGtext.update(BGimg);

		//texture dans le sprite
		sprite.setTexture(texture, false);
		BGsprt.setTexture(BGtext, false);


		win.draw(BGsprt);
		win.draw(sprite);

		win.display();
	}
}

sf::Color Simulation::HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI)
{
	//hue : 0 : red  1 : yellow  2 : green  3 : cyan  4 : blue  5 : purple  6 : red
	//hue  0 == 6   6 is one cycle rotation
	//saturation [0;1]
	//darkness [0;1]
	//alpha [0;1]

	double red = 0;
	double green = 0;
	double blue = 0;
	double hue = fmod(hueI, 6);

	if (hue >= 0 && hue < 1) {
		red = 255; green = hue * 255; blue = 0;
	}
	else if (hue >= 1 && hue < 2) {
		green = 255; red = 255 - ((hue - 1) * 255); blue = 0;
	}
	else if (hue >= 2 && hue < 3) {
		green = 255; blue = (hue - 2) * 255; red = 0;
	}
	else if (hue >= 3 && hue < 4) {
		blue = 255; green = 255 - ((hue - 3) * 255); red = 0;
	}
	else if (hue >= 4 && hue < 5) {
		blue = 255; red = (hue - 4) * 255; green = 0;
	}
	else if (hue >= 5 && hue < 6) {
		red = 255; blue = 255 - ((hue - 5) * 255); green = 0;
	}

	red = red + (255 - red) * satI;
	green = green + (255 - green) * satI;
	blue = blue + (255 - blue) * satI;

	red = red * darkI;
	green = green * darkI;
	blue = blue * darkI;

	return Color(static_cast<Uint8>(red), static_cast<Uint8>(green), static_cast<Uint8>(blue), static_cast<Uint8>(alphaI * 255));
}