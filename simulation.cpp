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
	nbBoule = 10;
	vector<Boule> boules;
}

void Simulation::Init() {
	boules.clear();
	for (int i = 0; i < nbBoule; i++)
	{
		boules.push_back(Boule());
		boules[i].Init(i);
		boules[i].Update();
	}
}

void Simulation::Print() {
	cout << to_string(win_height) << endl;
}

void Simulation::Update() {
	Show();
}

void Simulation::Show() {
	Color red;
	red.r = 255;
	Color green;
	green.g = 255;
	for (int i = 0; i < win_width; i++) {
		for (int j = 0; j < win_height; j++) {
			if ((i + j) % 100 == 0) {

				image.setPixel(i, j, red);
			}
			if ((i - j) % 100 == 0) {

				image.setPixel(i, j, green);
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