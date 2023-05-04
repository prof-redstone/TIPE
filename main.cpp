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
const int WIN_WIDTH = 700;//taille de la window
const int WIN_HEIGHT = 700;

int main()
{
    cout << "------- TIPE SIMULATION ---------" << endl;

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Fractal Generator");
    window.setTitle("Simulation TIPE");
    window.setKeyRepeatEnabled(false);//pour éviter les trucs bisard quand on utilise sur le clavier
    window.setVerticalSyncEnabled(true);//activation Vsinc

    
    simulation.UpdateWindow(window);//pour lui passer en parametre les parametres de la window, comme la taille.
    simulation.Init(0.001, 16, 0.5);//fonction de test

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        simulation.Update();
        //simulation.AddBall();

        //affichage du resultat
        simulation.Render(window);
    }

    return 0;
}