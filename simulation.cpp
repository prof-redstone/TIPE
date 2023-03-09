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
	vector<Boule> boules;
}

void Simulation::Init() {
	boules.clear();
	for (int i = 0; i < nbBoule; i++)
	{
		boules.push_back(Boule());
		boules[i].Init(i, i * 50, 50);
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
	DrawBoule();
}

void Simulation::DrawBoule() {
	cout << "ici 1" << endl;
	for (int i = 0; i < nbBoule; i++) { //indice de la boule
		int x = 50;
		int y = 50;
		int size = 30;
		for (int j = x - size; j < x + size; j++) {//coo x
			for (int k = y - size; k < y + size; k++) { //coo y
				double dist = sqrt( ((double)pow(j - x, 2)) + ((double)pow(k - y, 2)) );
				if ((j >= 0 && j <= win_width) && (k >= 0 && k <= win_height)){
					cout << to_string((double)size) << endl;
					if (dist <= (double)size) {
						Color red;
						red.r = 255;
						image.setPixel(j, k, red);
					}
					else {
						Color black;
						black.r = 255;
						image.setPixel(j, k, black);
					}
				}
			}
		}
	}
	cout << "fini !" << endl;
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