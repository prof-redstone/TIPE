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
	bool finish;

	double deltaTime;
	int time;//incremente de 1 dans Update

	//pour les boules
	int nbBoule;
	vector<Boule> boules;
	vector <int> boulesCol;


	//pour les brasseurs
	int nbBrasseur;
	vector<Brasseur> brasseurs;
	bool rotate;

	//pour le tirage
	int nbTirage;
	int nbTirageFait;
	double timebtwTirage;
	vector <int> resTirage;
	int detectorX;
	int detectorY;


	Simulation();

	void Init(double dt, int taille, double Bsize, double noise, int seed, int nbbras, double brasSize, double brasSpeed, int nbTirage, double timebtwTirage);//dt C[0.001, 0.003] , taille (nb triangulaire) [10, 16], noise [0,


	void Update();//fonction principale

	//fonctions pour les balles
	void ApplyForce();
	void ResolveCollision();
	void ResolveConstraint();	
	void UpdateBall();
	void AddBall();

	void Tirage();
	int Detector();

	void UpdateBrasseur();

	void UpdateWindow(sf::RenderWindow& win);


	void Render(sf::RenderWindow& win);
	void DrawBoule();
	void DrawBrasseur();
	void DrawDetector();
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};




#endif