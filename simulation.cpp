#include "simulation.h"
#include "boule.h"

using namespace sf;
using namespace std;



Simulation::Simulation() {

	win_width = 1000;//default value, use updateWindow to change this value
	win_height = 1000;

	//graphic part
	image.create(win_width, win_height, Color::Cyan);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture);


	//Boule part
	nbBoule = 1;
	deltaTime = 0.01;
	vector<Boule> boules;
}

void Simulation::Init() {
	boules.clear();
	for (int i = 0; i < nbBoule; i++)
	{
		boules.push_back(Boule());
		boules[i].Init(i, 50, 50, 10);
		boules[i].Update();
	}
}

void Simulation::Print() {
	cout << to_string(win_height) << endl;
}

void Simulation::Update() {
	for (int i = 0; i < nbBoule; i++){
		//Gravity Force 
		//am=f donc a=f/m
		double gravity = 1000;
		double yf = gravity*boules[i].weight;//foce sur l'axe x
		double yacc = yf / boules[i].weight; //acceleration sur l'axe x
		boules[i].yspeed += yacc * deltaTime;
		boules[i].ypos += boules[i].yspeed * deltaTime;
	}
}

void Simulation::Show() {
	DrawBoule();
}

void Simulation::DrawBoule() {
	for (int i = 0; i < nbBoule; i++) { //indice de la boule
		int x = (int)boules[i].xpos;
		int y = (int)boules[i].ypos;
		int r = (int)boules[i].size;
		for (int j = x - r; j < x + r; j++) {
			for (int k = y - r; k < y + r; k++) {
				if ((j >= 0 && j < win_width) && (k >= 0 && k < win_height)) {
					double dist = sqrt(((double)pow(j - x, 2)) + ((double)pow(k - y, 2)));
					if (dist <= (double)r) {
						Color red;
						red.r = 255;
						image.setPixel(j, k, red);
					}
				}
			}
		}
	}
}

void Simulation::UpdateWindow(sf::RenderWindow& win) {
	win_width = win.getSize().x;
	win_height = win.getSize().y;
	image.create(win_width, win_height, Color::Black);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture, true);
}


void Simulation::Render(sf::RenderWindow& win) {

	//creation de la texture par defaut
	//image dans la texture
	texture.create(win_width, win_height);
	texture.update(image);

	//texture dans le sprite
	sprite.setTexture(texture, false);

	win.draw(sprite);

}