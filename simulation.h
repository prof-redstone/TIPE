#ifndef SIMULATION_H
#define SIMULATION_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>  //to initialize random


class Simulation {
public:


	//varaible pour la fonction render
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	int win_width;//taille de la window
	int win_height;

	Simulation();

	void print();

	void update();

	void show();

	void updateWindow(sf::RenderWindow& win);


	void render(sf::RenderWindow& win);

};




#endif