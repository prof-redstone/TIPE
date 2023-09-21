#include <SFML/Graphics.hpp>
#include <iostream> //pour ecrire dans la console.
#include <vector> //pour les tableau dynamique
#include <cmath> //pour tous les modules de math
#include <ctime>
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
double noise = 0.1; //quantite de bruit dans le positionnement des boules a l'etat initial
int nbFrameSkip = 10; //r�duis le nombre d'image ce qui accelere la simulation
double bouleSize = 30; //rayon de la taille des boules en pixel dans la simulation 
int nbBrasseur = 40; 
double brasseurSize = 12; //rayon de la taille des brasseurs en pixel dans la simulation 
double brasseurSpeed = 0.7; //vitesse de rotation des brasseurs en rad/frame
int seed = 2; //seed pour la position des balles
int nbTirage = 5;
double timebtwTirage = 500; //unite random

void getParam(int argc, char* argv[]);//y'a pas de .h

void main(int argc, char* argv[]){
    getParam(argc, argv);

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "TIPE SIMULATION");
    window.setTitle("Simulation TIPE Tom Demagny");
    window.setKeyRepeatEnabled(false);//pour �viter les trucs bisard quand on utilise sur le clavier
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
        simulation.Update();
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

void getParam(int argc, char* argv[]) {

    //cout << "Nombre de parametre : " + to_string(argc) << endl;


    for (int i = 1; i < argc-1; i++) {//-1 pour faire +1 
        //cout << "param " + to_string(i) + " : " << argv[i] << endl;
        if (strcmp(argv[i], "seed") == 0) {
             seed = stoi(argv[i + 1]); 
        }
        if (strcmp(argv[i], "nbtirage") == 0) {
            nbTirage = stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "nbboule") == 0) {
            nbBoule = stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "sizeboule") == 0) {
            bouleSize = stod(argv[i + 1]);
        }
        if (strcmp(argv[i], "noise") == 0) {
            noise = stod(argv[i + 1]);
        }
        if (strcmp(argv[i], "brasseurSpeed") == 0) {
            brasseurSpeed = stod(argv[i + 1]);
        }
        if (strcmp(argv[i], "inttirage") == 0) {
            timebtwTirage = stod(argv[i + 1]);
        }
        if (strcmp(argv[i], "nbFrameSkip") == 0) {
            nbFrameSkip = stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "dt") == 0) {
            dt = stod(argv[i + 1]);
        }
        if (strcmp(argv[i], "size") == 0) {
            WIN_WIDTH = stoi(argv[i + 1]);
            WIN_HEIGHT = stoi(argv[i + 1]);
        }
    }

    cout << "Delta Time : " + to_string(dt) << endl; //devra etre fixe
    cout << "Nombre de boules : " + to_string(nbBoule) << endl;
    cout << "Taille boules : " + to_string(bouleSize) << endl; //devra etre fixe
    cout << "Seed for noise : " + to_string(seed) << endl;
    cout << "Noise factor : " + to_string(noise) << endl;
    //cout << "Nombre brasseur : " + to_string(nbBrasseur) << endl; //devra etre fixe
    cout << "Brasseur speed : " + to_string(brasseurSpeed) << endl; //devra etre fixe
    //cout << "Brasseur size : " + to_string(brasseurSize) << endl; //devra etre fixe
    cout << "Square size : " + to_string(WIN_WIDTH) << endl; //devra etre fixe
    cout << "Nombre frame skip :" + to_string(nbFrameSkip) << endl; //devra etre fixe
    cout << "Nombre tirage : " + to_string(nbTirage) << endl;
    cout << "intervalle tirage : " + to_string(timebtwTirage) << endl;
}