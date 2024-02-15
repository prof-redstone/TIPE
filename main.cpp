#include <SFML/Graphics.hpp>
#include <iostream> //pour ecrire dans la console.
#include <vector> //pour les tableau dynamique
#include <cmath> //pour tous les modules de math
#include <chrono> //pour avoir le temps pour la fonction al�atoire
#include <ctime>  //pour avoir le temps pour la fonction al�atoire

#include "main.h"
#include "simulation.h"

//name space
using namespace sf;
using namespace std;

Simulation simulation;

//constantes
int WIN_WIDTH = 800;//taille de la window 800*800
int WIN_HEIGHT = 800;

int nbFrameSkip = 50; //r�duis le nombre d'image ce qui accelere la simulation
double dt = 0.0005; //delta time pourune boucle de simulation 0.0005

__int64 seed = 2; //seed pour le random, valeur qui changera si rndseed est true
bool rndSeed = true;
double noise = 0.0; //quantite de bruit dans le positionnement des boules a l'etat initial //0.9
double bounceNoiseBall = 0.1; //0.1
double bounceNoiseBrass = 0.1;//0.1

int nbBoule = 7; //nombre triagulaire de boule dans la simulation, 15 max, 7 c'est bien
double bouleSize = 30.; //rayon de la taille des boules en pixel dans la simulation //30

int nbBrasseur = 40; //40
double brasseurSize = 12.; //rayon de la taille des brasseurs en pixel dans la simulation //12
double brasseurSpeed = 0.7; //vitesse de rotation des brasseurs en rad/frame //0.7

int nbTirage = 10; //nombre de boule total tir�e avant la fin du programme
double timebtwTirage = 10.; //unite seconde un peu proportionnelle, 10 c'est bien
double timeBeforStart = 5.; //5 c'est bien

bool brasseurRNDpos = false; //true
bool bouleRNDpos = true; //true

void getParam(int argc, char* argv[]);//y'a pas de .h
double GetPreciseTime();

void main(int argc, char* argv[]){
    if (rndSeed) {
        seed = (__int64)(GetPreciseTime());
    }
    getParam(argc, argv);//pou recuperer tous les parametre de python

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "TIPE SIMULATION");
    String title = "Simulation TIPE Tom Demagny";
    window.setTitle(title);
    window.setKeyRepeatEnabled(false);//pour �viter les trucs bisard quand on utilise sur le clavier
    window.setVerticalSyncEnabled(true);//activation Vsinc

    
    simulation.UpdateWindow(window);//pour lui passer en parametre les parametres de la window, comme la taille.
    simulation.Init(dt, nbBoule, bouleSize, noise, seed, nbBrasseur, brasseurSize, brasseurSpeed, nbTirage, timebtwTirage, timeBeforStart, brasseurRNDpos, bouleRNDpos, bounceNoiseBall, bounceNoiseBrass);//fonction de test



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
        if (strcmp(argv[i], "RNDBoulePos") == 0) {
            bouleRNDpos = stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "RNDbrassPos") == 0) {
            brasseurRNDpos = stoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "timeBeforStart") == 0) {
            timeBeforStart = stoi(argv[i + 1]);
        }
    }

    cout << "Seed : " + to_string(seed) << endl;
    cout << "Pos init noise factor : " + to_string(noise) << endl;
    cout << "Ball bounce noise factor : " + to_string(bounceNoiseBall) << endl;
    cout << "Brass bounce noise factor : " + to_string(bounceNoiseBrass) << endl;
    cout << "Delta Time : " + to_string(dt) << endl; //devra etre fixe
    cout << "Nombre de boules : " + to_string(nbBoule) << endl;
    cout << "Taille boules : " + to_string(bouleSize) << endl; //devra etre fixe
    cout << "Nombre brasseur : " + to_string(nbBrasseur) << endl; //devra etre fixe
    cout << "Brasseur speed : " + to_string(brasseurSpeed) << endl; //devra etre fixe
    cout << "Brasseur size : " + to_string(brasseurSize) << endl; //devra etre fixe
    cout << "Square size : " + to_string(WIN_WIDTH) << endl; //devra etre fixe
    cout << "Nombre frame skip : " + to_string(nbFrameSkip) << endl; //devra etre fixe
    cout << "Nombre tirage : " + to_string(nbTirage) << endl;
    cout << "Intervalle tirage : " + to_string(timebtwTirage) << endl;
    cout << "Temps avant debut : " + to_string(timeBeforStart) << endl;
    cout << "Position RND boules : " + to_string(bouleRNDpos) << endl;
    cout << "Position RND brasseur : " + to_string(brasseurRNDpos) << endl;
}

double GetPreciseTime() {

    // R�cup�re le temps actuel
    auto now = std::chrono::system_clock::now();

    // Convertit le temps en seconde
    std::chrono::duration<double> duration = now.time_since_epoch();

    // Convertit le temps en centi�mes de seconde
    std::chrono::duration<double, std::ratio<1, 1>> hundredths = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(duration);

    // Convertit le nombre en cha�ne de caract�res
    std::string secChar = std::to_string(hundredths.count());


    // Convertit la cha�ne de caract�res en nombre double
    double hundredths_double = std::stod(secChar.c_str());
    double millis = hundredths_double*1000.;

    // Affiche le temps
    //std::cout << "Le temps actuel est : " << secChar << " seconde" << std::endl;
    //std::cout << "Le temps actuel est : " << millis << " seconde" << std::endl;
    return millis;
}