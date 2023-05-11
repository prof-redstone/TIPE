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
const int nbBoule = 18; //nombre triagulaire de boule dans la simulation, 15 max
const double noise = 3; //quantite de bruit dans le positionnement des boules a l'etat initial
const int nbFrameSkip = 7; //réduis le nombre d'image ce qui accelere la simulation
const double bouleSize = 10; //rayon de la taille des boules en pixel dans la simulation 
const int nbBrasseur = 5; 
const double brasseurSize = 10; //rayon de la taille des brasseurs en pixel dans la simulation 
const double brasseurSpeed = 0.9; //vitesse de rotation des brasseurs en rad/frame
const int seed = 3; //seed pour la position des balles

int main(){
    cout << "------- TIPE SIMULATION ---------" << endl;
    cout << "Delta Time : " + to_string(dt) << endl;
    cout << "Nombre de boules : " + to_string(nbBoule) << endl;
    cout << "taille boules : " + to_string(bouleSize) << endl;
    cout << "seed for noise : " + to_string(seed) << endl;
    cout << "Noise factor : " + to_string(noise) << endl;
    cout << "Nombre brasseur :" + to_string(nbBrasseur) << endl;
    cout << "Brasseur speed : " + to_string(brasseurSpeed) << endl;
    cout << "Brasseur size : " + to_string(brasseurSize) << endl;
    cout << "Nombre frame skip :" + to_string(nbFrameSkip) << endl;

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

    return 0;
}