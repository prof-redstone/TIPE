#include <SFML/Graphics.hpp>
#include <iostream> //pour ecrire dans la console.
#include <vector> //pour les tableau dynamique
#include <cmath> //pour tous les modules de math
#include <chrono> //pour avoir le temps pour la fonction aléatoire
#include <ctime>  //pour avoir le temps pour la fonction aléatoire

#include "main.h"
#include "simulation.h"

//name space
using namespace sf;
using namespace std;

Simulation simulation;

//constantes
int WIN_WIDTH = 800;//taille de la window 800*800
int WIN_HEIGHT = 800;
int nbFrameSkip = 15; //réduis le nombre d'image ce qui accelere la simulation
double dt = 0.0005; //delta time pourune boucle de simulation 0.0005
int nbBoule = 8; //nombre triagulaire de boule dans la simulation, 15 max
double noise = 0.5; //quantite de bruit dans le positionnement des boules a l'etat initial //0.9
double bouleSize = 30; //rayon de la taille des boules en pixel dans la simulation //30
int nbBrasseur = 40; //40
double brasseurSize = 12; //rayon de la taille des brasseurs en pixel dans la simulation //12
double brasseurSpeed = 0.7; //vitesse de rotation des brasseurs en rad/frame //0.7
__int64 seed = 2; //seed pour la position des balles
int nbTirage = 40;
double timebtwTirage = 5; //unite seconde un peu proportionnelle
double timeBeforStart = 7;
bool brasseurRNDpos = true;
bool bouleRNDpos = true;

void getParam(int argc, char* argv[]);//y'a pas de .h
double GetPreciseTime();

void main(int argc, char* argv[]){
    double time = GetPreciseTime();
    seed = (__int64)(time);
    cout << "seed with millis : " << seed << endl;
    getParam(argc, argv);

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "TIPE SIMULATION");
    window.setTitle("Simulation TIPE Tom Demagny");
    window.setKeyRepeatEnabled(false);//pour éviter les trucs bisard quand on utilise sur le clavier
    window.setVerticalSyncEnabled(true);//activation Vsinc

    
    simulation.UpdateWindow(window);//pour lui passer en parametre les parametres de la window, comme la taille.
    simulation.Init(dt, nbBoule, bouleSize, noise, seed, nbBrasseur, brasseurSize, brasseurSpeed, nbTirage, timebtwTirage, timeBeforStart, brasseurRNDpos, bouleRNDpos);//fonction de test



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
    cout << "Intervalle tirage : " + to_string(timebtwTirage) << endl;
    cout << "Temps avant debut : " + to_string(timeBeforStart) << endl;
    cout << "Position RND boules : " + to_string(bouleRNDpos) << endl;
    cout << "Position RND brasseur : " + to_string(brasseurRNDpos) << endl;
}

double GetPreciseTime() {

    // Récupère le temps actuel
    auto now = std::chrono::system_clock::now();

    // Convertit le temps en seconde
    std::chrono::duration<double> duration = now.time_since_epoch();

    // Convertit le temps en centièmes de seconde
    std::chrono::duration<double, std::ratio<1, 1>> hundredths = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(duration);

    // Convertit le nombre en chaîne de caractères
    std::string secChar = std::to_string(hundredths.count());


    // Convertit la chaîne de caractères en nombre double
    double hundredths_double = std::stod(secChar.c_str());
    double millis = hundredths_double*1000.;

    // Affiche le temps
    //std::cout << "Le temps actuel est : " << secChar << " seconde" << std::endl;
    //std::cout << "Le temps actuel est : " << millis << " seconde" << std::endl;
    return millis;
}