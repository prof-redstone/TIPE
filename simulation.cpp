#include "simulation.h"

#include "func.h"
#include "boule.h"
#include "brasseur.h"
#include <chrono>//pour le getTime precise

using namespace sf;
using namespace std;


//pour initialiser l'objet simulation
Simulation::Simulation() {

	win_width = 1000; //default value, use updateWindow to change this value
	win_height = 1000;

	finish = false;

	//graphic part
	image.create(win_width, win_height, Color::Cyan);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture);


	//Boule part
	deltaTime = 0.005;//par defaut (a changer dans le Init)
	nbFrameSkip = 3;
	time = 0;//augmente de 1 a chaque update
	seed = 0; 
	brasseurRNDpos = false;
	bouleRNDpos = false;
	nbBoule = 0; //changera dans Init
	vector < Boule > boules;


	nbBrasseur = 0; //changera dans Init
	vector < Brasseur > brasseurs;
	rotate = false;

	//pour le tirage
	nbTirage = 0; //nombre total de tirage a faire
	nbTirageFait = 0; //pour garder le nombre de tirage deja execute 
	timebtwTirage = 1000; //change de le INIT
	timeBeforStart = 1000;
	vector <int> resTirage;
	detectorX = win_width / 2;
	detectorY = win_height;

}

//pour initialiser les parametre, et remetre a 0 les boules
void Simulation::Init(double dt, int taille,double Bsize, double IPosNoise,int Iseed, int nbbras, double brasSize, double brasSpeed, int nbTir, double timebtwTir, double itimeBeforStart,bool ibrasseurRNDpos, bool ibouleRNDpos, double IbounceNoiseBall, double IbounceNoiseBrass) {
	deltaTime = dt;
	nbBrasseur = nbbras;
	nbTirage = nbTir;
	timebtwTirage = timebtwTir;
	finish = false;
	rotate = false;
	brasseurRNDpos = ibrasseurRNDpos;
	bouleRNDpos = ibouleRNDpos;
	timeBeforStart = itimeBeforStart;
	seed = Iseed;
	PosNoise = IPosNoise;
	bounceNoiseBall = IbounceNoiseBall;
	bounceNoiseBrass = IbounceNoiseBrass;
	brassMaxSpeed = brasSpeed * dt;

	//pour mettre les résultats
	for (int i = 0; i < nbTirage; i++){
		resTirage.push_back(-1); //-1 = pas de valeur
	}

	//pour generer les boules au début de la simulation
	//generation en forme de triangle de taille taille
	double Bmarge = 5; //minimum 1px d'espace entre les boules pour eviter pb de colision
	double centerX = win_width / 2;
	double centerY = win_height / 2 - (taille /2)*(Bmarge + Bsize*2);
	boules.clear();
	nbBoule = 0;
	for (int t = taille; t >= 0; t--){
		for (int i = 0; i <= t; i++) {
			boules.push_back(Boule());
			//bruit dans le placement des boules
			double xnoise = rnd(seed, i)* PosNoise;
			double ynoise = rnd(seed + 1, i)* PosNoise;
			boules[nbBoule].Init(nbBoule, centerX + ((Bsize*2 + Bmarge) * i) - ((Bsize*2 + Bmarge)*0.5*t) + xnoise, t * (Bsize*2 + Bmarge)*0.87 + centerY + ynoise, Bsize);
			nbBoule++;
			
		}
	}

	if (bouleRNDpos) {//on melange les positions des boules
		boules = shuffle(boules, nbBoule , seed);
	}


	//pour generer les brasseurs, taille position et rayon
	double sizeBrasseur = brasSize; // rayon du brasseur
	double angInitRnd = 0;
	if (brasseurRNDpos) {//la postion de depart est random, 0 sinon
		angInitRnd = rnd(seed, 1) * (2 * 3.1415);
	}
	for (int i = 0; i < nbBrasseur; i++){
		brasseurs.push_back(Brasseur()) ;
		double angsec = (2 * 3.1415) / nbBrasseur;
		brasseurs[i].Init(i * angsec + angInitRnd, (win_width/2)-sizeBrasseur, sizeBrasseur, brassMaxSpeed, win_width / 2, win_height / 2);
	}

}


void Simulation::UpdateWindow(sf::RenderWindow& win) { //appele une fois au début du prog pour initialiser la taille de la fenettre, ou en cours pour redimmentionner la fenettre
	win_width = win.getSize().x;
	win_height = win.getSize().y;

	detectorX = win_width / 2;
	detectorY = win_height - 10;

	Color LayerColor = Color(0, 0, 0, 0);
	image.create(win_width, win_height, LayerColor);
	BGimg.create(win_width, win_height, LayerColor);
	sprite.setTexture(texture, true);

	//loadFont();

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
	BGtext.create(win_width, win_height);
	BGtext.update(BGimg);
	BGsprt.setTexture(BGtext, false);
}


//main function of program, call at each frame
void Simulation::Update() {
	/*pour les balles :
		ordre exection des etapes :
		1)gravity
		2)check collision
		3)faire les contrainte de collision, bordure, brasseur
		4)update la position
	*/
	ApplyForce();
	ResolveCollision2(); //boule a element fixe
	ResolveConstraint2(); //boule a boule
	UpdateBall();

	Routine();
	time++;
}

void Simulation::Routine() {
	static int timerR = 0;
	timerR++;
	//timeBeforStart ~= 7
	//timeSuffle ~= 5
	double accTime = 4.;
	static int state = 0; //initialisation : 0   acceleration : 1    suffle : 2   deceleration : 3    tirage : 4
	float secTime = (timerR * deltaTime);
	if (state == 1 || state == 2) {
		UpdateBrasseur(1);
	}
	else {
		UpdateBrasseur(0);
	}

	if (state == 4) {
		Tirage();
	}

	if (nbTirageFait < nbTirage) {
		if (state == 0 && secTime >= timeBeforStart) {
			cout << "acceleration" << endl;
			timerR = 0;
			state = 1;
			return;
		}
		if (state == 1 && secTime >= accTime) {
			cout << "suffle" << endl;
			timerR = 0;
			state = 2;
			return;
		}
		if (state == 2 && secTime >= 7) {
			cout << "deceleration" << endl;
			timerR = 0;
			state = 3;
			return;
		}
		if (state == 3 && secTime >= accTime) {
			cout << "tirage" << endl;
			timerR = 0;
			state = 4;
			return;
		}
		if (state == 4) {
			cout << "restart" << endl;
			timerR = 0;
			state = 1;
			return;
		}
	}
}


void Simulation::Tirage() {

	if (nbTirageFait < nbTirage) {
		float step = ((time * deltaTime) - timeBeforStart) / timebtwTirage;
		if (step > nbTirageFait + 1) {
			int i = (int)floor(step)-1;

			if (resTirage[i] == -1) {
				int res = Detector();
				if (res != -1) {//si il y a plus de boule
					boules[res].tire = true;
				}
				resTirage[i] = res;
				nbTirageFait++;
			}
		}
	}else {
		finish = true;
	}
}

int Simulation::Detector() {
	int indexMin = -1;//-1 si y'a plus de boule
	double distMin = (double)100000000000000; //max double
	for (int i = 0; i < nbBoule; i++)
	{
		double distX = boules[i].xpos - detectorX;
		double disty = boules[i].ypos - detectorY;
		double dist = sqrt(distX*distX + disty*disty);
		if (dist < distMin && boules[i].tire==false) {//pour pas retirer la meme
			distMin = dist;
			indexMin = i;
		}
	}
	return indexMin;
}

//appliquer les différente force du systeme sur les balles
void Simulation::ApplyForce(){
	//application de la gravité et autre force potentielle
	for (int i = 0; i < nbBoule; i++) {
		//Gravity Force 
		//am=f donc a=f/m
		double gravity = 1000; //1000
		double yf = gravity * boules[i].weight; //foce sur l'axe x
		boules[i].yacc = yf / boules[i].weight; //acceleration sur l'axe x
	}
}

//ce qui fait que ça fonctionne, pour gerer les collisions avec les différentes balles
void Simulation::ResolveCollision(){
	//check collision entre les boulles
	const double repCoef = 0.75; //0.75
	for (int i = 0; i < nbBoule; i++) { //objet 1
		for (int j = i + 1; j < nbBoule; j++) { // objet 2 different de objet 1, pour faire une paire
			double distx = boules[i].xpos - boules[j].xpos;
			double disty = boules[i].ypos - boules[j].ypos;
			double dist2 = distx * distx + disty * disty; //pour opti
			double distmin = boules[i].size + boules[j].size; 

			//check overlapping, and existance
			if (dist2 < distmin * distmin && boules[i].tire == false && boules[j].tire == false) {
				double dist1 = sqrt(dist2);
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

void Simulation::ResolveCollision2() {//collision entre les boules seulement
	const double bounceRate = 1.;
	for (int i = 0; i < nbBoule; i++) { //objet 1
		for (int j = i + 1; j < nbBoule; j++) { // objet 2 different de objet 1, pour faire une paire
			double distx = boules[i].xpos - boules[j].xpos;
			double disty = boules[i].ypos - boules[j].ypos;
			double dist2 = distx * distx + disty * disty; //pour opti
			double distmin = boules[i].size + boules[j].size;

			//check overlapping, and existance
			if (dist2 < distmin * distmin && boules[i].tire == false && boules[j].tire == false) {
				//double dist1 = sqrt(dist2);
				double rnddir = (rnd(seed, i*j * time) * 2. - 1.) * bounceNoiseBall;
				double distToMove = distmin - sqrt(dist2);
				double angle = atan2(disty, distx) + rnddir;

				const double massRatio1 = boules[j].size / (boules[i].size + boules[j].size);
				const double massRatio2 = boules[i].size / (boules[i].size + boules[j].size);
				boules[i].xpos += cos(angle)*distToMove * massRatio1;
				boules[i].ypos += sin(angle)*distToMove * massRatio1;
				boules[j].xpos -= cos(angle)*distToMove * massRatio2;
				boules[j].ypos -= sin(angle)*distToMove * massRatio2;
			}
		}
	}

}

void Simulation::ResolveConstraint(){
	//application des contrainte de mouvement :
	//zone de mouvement : cercle de taille de la fenettre centre (win_width = radius)

	//Collision avec la bordure
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

	//collision avec les brasseurs
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

void Simulation::ResolveConstraint2() {
	//application des contrainte de mouvement :
	//zone de mouvement : cercle de taille de la fenettre centre (win_width = radius)

	//Collision avec la bordure
	const double zoneRadius = win_width / 2;//rayon de la zone libre
	const double bounceRate = 1;
	for (int i = 0; i < nbBoule; i++) {
		double distx = boules[i].xpos - zoneRadius;//distance de la boule au centre 
		double disty = boules[i].ypos - zoneRadius;
		double dist = sqrt(distx * distx + disty * disty);
		double distmin = zoneRadius - boules[i].size;
		if (dist > distmin && boules[i].tire == false) {
			double rnddir = (rnd(seed, i * time) * 2. - 1.) * bounceNoiseBrass;
			double distToMove = distmin - dist;
			double angle = atan2(disty, distx) + rnddir; 
			boules[i].xpos += cos(angle) * distToMove;
			boules[i].ypos += sin(angle) * distToMove;
		}

	}

	//collision avec les brasseurs
	for (int i = 0; i < nbBoule; i++) {
		for (int j = 0; j < nbBrasseur; j++) {
			double distx = boules[i].xpos - brasseurs[j].xpos;//distance entre les 2 cercles 
			double disty = boules[i].ypos - brasseurs[j].ypos;
			double dist = sqrt(distx * distx + disty * disty);
			double distmin = brasseurs[j].size + boules[i].size;
			if (dist < distmin && boules[i].tire == false){
				double rnddir = (rnd(seed, i * time) * 2. - 1.) * bounceNoiseBrass;
				double distToMove = distmin - dist;
				double angle = atan2(disty, distx) + rnddir;
				boules[i].xpos += cos(angle) * distToMove;
				boules[i].ypos += sin(angle) * distToMove;
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

void Simulation::UpdateBrasseur(int acc) {
	double springForce = 0.002;
	double desiredSpeed = acc ? brassMaxSpeed + brassMaxSpeed*0.1*(perlin_noise(seed, ((float)time) / 1000)) : 0.0;
	double actuSpeed = brasseurs[0].speedAng;
	for (int i = 0; i < nbBrasseur; i++) {
		brasseurs[i].speedAng += (desiredSpeed - actuSpeed) * springForce;
		brasseurs[i].Update();
	}
}


void Simulation::DrawBoule() { //pour afficher les boules une par une sur l'image
	for (int i = 0; i < nbBoule; i++) { //indice de la boule
		if (boules[i].tire == false) { //on affiche pas les boules deja tire
			int x = (int)boules[i].xpos;
			int y = (int)boules[i].ypos;
			int r = (int)boules[i].size;
			Color col = HSLtoRGB(((double)boules[i].index) / 8.0, 0.1, 1, 1);
			for (int j = 0; j < nbTirage; j++)
			{
				if (resTirage[j] == i) {
					col = HSLtoRGB(((double)boules[i].index) / 8.0, 0, 0.1, 0.0);

				}
			}
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

void Simulation::DrawDetector() {
	int x = detectorX;
	int y = detectorY;
	int r = 10;

	Color col = Color(250, 0, 0);
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

void Simulation::DrawTimer() {
	int xs = win_width*0.85;
	int ys = 30;

	int x = win_width/2 - xs/2;
	int y = win_height/2 - ys / 2;

	double pourcentage = (time * deltaTime) / timeBeforStart;

	if (pourcentage > 1.) {
		return;
	}

	xs *= 1-pourcentage;

	Color col = Color(250*(1-pourcentage), 250 * pourcentage, 0);
	for (int j = x; j < x + xs; j++) {
		for (int k = y; k < y + ys; k++) {
			if ((j >= 0 && j < win_width) && (k >= 0 && k < win_height)) {
				image.setPixel(j, k, col);
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
		DrawDetector();
		DrawTimer();

		//creation de la texture par defaut
		//image dans la texture
		texture.create(win_width, win_height);
		texture.update(image);

		//texture dans le sprite
		sprite.setTexture(texture, false);


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