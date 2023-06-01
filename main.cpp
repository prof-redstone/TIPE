#include <SFML/Graphics.hpp>
#include <iostream> //pour ecrire dans la console.
#include <vector> //pour les tableau dynamique
#include <cmath> //pour tous les modules de math
#include "main.h"
#include "simulation.h"

//name space
using namespace sf;
using namespace std;

Simulation simulation;

//constantes
const int WIN_WIDTH = 800;//taille de la window
const int WIN_HEIGHT = 800;
const double dt = 0.0005; //delta time pourune boucle de simulation
const int nbBoule = 8; //nombre triagulaire de boule dans la simulation, 15 max
const double noise = 3; //quantite de bruit dans le positionnement des boules a l'etat initial
const int nbFrameSkip = 8; //réduis le nombre d'image ce qui accelere la simulation
const double bouleSize = 30; //rayon de la taille des boules en pixel dans la simulation 
const int nbBrasseur = 40; 
const double brasseurSize = 12; //rayon de la taille des brasseurs en pixel dans la simulation 
const double brasseurSpeed = 0.7; //vitesse de rotation des brasseurs en rad/frame
const int seed = 3; //seed pour la position des balles

void main(int argc, char* argv[]){
    cout << "Nombre de parametre : " + to_string(argc) << endl;
    for (int i = 0; i < argc; i++){
        cout << "param " + to_string(i) + " : " << argv[i] << endl;
    }
    cout << "Delta Time : " + to_string(dt) << endl; //devra etre fixe
    cout << "Nombre de boules : " + to_string(nbBoule) << endl;
    cout << "Taille boules : " + to_string(bouleSize) << endl; //devra etre fixe
    cout << "Seed for noise : " + to_string(seed) << endl;
    cout << "Noise factor : " + to_string(noise) << endl;
    cout << "Nombre brasseur : " + to_string(nbBrasseur) << endl; //devra etre fixe
    cout << "Brasseur speed : " + to_string(brasseurSpeed) << endl; //devra etre fixe
    cout << "Brasseur size : " + to_string(brasseurSize) << endl; //devra etre fixe
    cout << "Square size : " + to_string(WIN_WIDTH) << endl; //devra etre fixe
    cout << "Nombre frame skip :" + to_string(nbFrameSkip) << endl; //devra etre fixe

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "TIPE SIMULATION");
    window.setTitle("Simulation TIPE");
    window.setKeyRepeatEnabled(false);//pour éviter les trucs bisard quand on utilise sur le clavier
    window.setVerticalSyncEnabled(true);//activation Vsinc

    
    simulation.UpdateWindow(window);//pour lui passer en parametre les parametres de la window, comme la taille.
    simulation.Init(dt, nbBoule, bouleSize, noise, seed, nbBrasseur, brasseurSize, brasseurSpeed);//fonction de test

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (int i = 0; i < nbFrameSkip+1; i++){//on fait plusieur fois avec le nombre de frame skip
            simulation.Update();
        }

        //affichage du resultat
        simulation.Render(window);
    }

    cout << "Result : " << to_string(5) << endl;

    return ;
}