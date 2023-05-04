#ifndef SIMULATION_H
#define SIMULATION_H
#include "boule.h"
#include "brasseur.h"
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
	sf::Image BGimg;
	sf::Texture BGtext;
	sf::Sprite BGsprt;

	int win_width;//taille de la window
	int win_height;
	int nbFrameSkip = 2;

	double deltaTime;
	int time;//incremente de 1 dans Update

	//pour les boules
	int nbBoule;
	vector<Boule> boules;


	//pour les brasseurs
	int nbBrasseur;
	vector<Brasseur> brasseurs;


	Simulation();

	void Init(double dt, int taille, double noise);//dt C[0.001, 0.003] , taille (nb triangulaire) [10, 16], noise [0,


	void Update();//fonction principale

	//fonctions pour les balles
	void ApplyForce();
	void ResolveCollision();
	void ResolveConstraint();	
	void UpdateBall();
	void AddBall();

	void UpdateBrasseur();

	void UpdateWindow(sf::RenderWindow& win);


	void Render(sf::RenderWindow& win);
	void DrawBoule();
	void DrawBrasseur();
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};




#endif