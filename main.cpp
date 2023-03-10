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
const int WIN_WIDTH = 500;//taille de la window
const int WIN_HEIGHT = 500;

int main()
{
    cout << "------- TIPE SIMULATION ---------" << endl;

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Fractal Generator");
    window.setTitle("Simulation TIPE");
    window.setKeyRepeatEnabled(false);//pour ?viter les trucs bisard quand on utilise sur le clavier
    window.setVerticalSyncEnabled(true);//activation Vsinc

    
    simulation.Init();//fonction de test
    simulation.UpdateWindow(window);//pour lui passer en parametre les parametre de la window, comme la taille.

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update();

        //affichage du resultat
        window.clear();
        simulation.Render(window);
        window.display();
    }

    return 0;
}