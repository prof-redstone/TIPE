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

	int win_width;//varaible global
	int win_height;
	bool finish;
	bool brasseurRNDpos;
	double brassMaxSpeed;
	bool bouleRNDpos;
	double timeBeforStart;
	__int64 seed;
	double PosNoise;
	double bounceNoiseBall;
	double bounceNoiseBrass;

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

	void Init(double dt, int taille, double Bsize, double Inoise, int Iseed, int nbbras, double brasSize, double brasSpeed, int nbTirage, double timebtwTirage, double itimeBeforStart, bool brasseurRNDpos, bool bouleRNDpos,double IbounceNoiseBall, double IbounceNoiseBass);//dt C[0.001, 0.003] , taille (nb triangulaire) [10, 16], noise [0,


	void Update();//fonction principale

	//fonctions pour les balles
	void ApplyForce();
	void ResolveCollision(); 
	void ResolveCollision2();
	void ResolveConstraint();
	void ResolveConstraint2();
	void UpdateBall();
	void AddBall();
	void Routine();
	void Tirage();
	int Detector();

	void UpdateBrasseur(int acc);

	void UpdateWindow(sf::RenderWindow& win);


	void Render(sf::RenderWindow& win);
	void DrawBoule();
	void DrawBrasseur();
	void DrawDetector();
	void DrawTimer();
	void DisplayDebugInfo(sf::RenderWindow& win);
	sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI);

};




#endif