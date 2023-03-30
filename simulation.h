#ifndef SIMULATION_H
#define SIMULATION_H
#include "boule.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>  //to initialize random


using namespace sf;
using namespace std;


class Simulation {
public:


	//varaible pour la fonction render
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	int win_width;//taille de la window
	int win_height;

	//pour les boules
	int nbBoule;
	double deltaTime;
	vector<Boule> boules;

	int time;

	Simulation();

	void Init();

	void Print();

	void Update();

	void ApplyForce();
	void ResolveCollision();
	void ResolveConstraint();
	void UpdateBall();
	void AddBall();

	void Show();

	void UpdateWindow(sf::RenderWindow& win);


	void Render(sf::RenderWindow& win);
	void DrawBoule();

};




#endif