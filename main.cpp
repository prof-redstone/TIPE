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
int WIN_WIDTH = 800;//taille de la window
int WIN_HEIGHT = 800;
double dt = 0.001; //delta time pourune boucle de simulation 0.0005
int nbBoule = 8; //nombre triagulaire de boule dans la simulation, 15 max
double noise = 3; //quantite de bruit dans le positionnement des boules a l'etat initial
int nbFrameSkip = 10; //réduis le nombre d'image ce qui accelere la simulation
double bouleSize = 30; //rayon de la taille des boules en pixel dans la simulation 
int nbBrasseur = 40; 
double brasseurSize = 12; //rayon de la taille des brasseurs en pixel dans la simulation 
double brasseurSpeed = 0.7; //vitesse de rotation des brasseurs en rad/frame
int seed = 4; //seed pour la position des balles
int nbTirage = 5;
double timebtwTirage = 500; //unite random

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
    simulation.Init(dt, nbBoule, bouleSize, noise, seed, nbBrasseur, brasseurSize, brasseurSpeed, nbTirage, timebtwTirage);//fonction de test

    while (window.isOpen() && !simulation.finish)
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

    //affichage du resultat dans la console
    cout << "Result : [";
    for (int i = 0; i < nbTirage; i++)
    {
        cout << to_string(simulation.resTirage[i]);
        if (i < nbTirage - 1) {
            cout << ",";
        }
    }
    cout << "]";
    cout << endl;

    return ;
}