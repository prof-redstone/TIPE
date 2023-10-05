
#include <iostream> //pour ecrire dans la console.
#include <random>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <string>
#include "boule.h"


using namespace sf;
using namespace std;


Font font; //from sfml
//charger la police d'ecriture
void loadFont() {
    if (!font.loadFromFile("font/poppins.ttf")) {
        cout << "Ereur de charment de la font " << "font.ttf" << endl;
    }
}

//si y'a besoins d'afficher un texte c'est près avec une police d'écriture 
void SetText(Text& txt, string str = "", Color col = Color::White, int size = 26) {
    /*Text txt;
    SetText(txt, "Hello World", Color::Green, 50);
    txt.setStyle(Text::Bold | Text::Underlined);*/
    txt.setFont(font);
    txt.setString(str);
    txt.setFillColor(col);
    txt.setCharacterSize(size);
}

int PRNG(int seed, int index)
{
    // our initial starting seed is 5323
    static unsigned int nSeed = seed*5323 + index*8647;

    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 32767
    return nSeed % 32767;
}

double rnd(int seed, int index) {
    int res = PRNG(seed, index) % 10000;
    return ((double)res) / 10000;
}

vector <Boule> shuffle(vector <Boule> tab, int n,int seed) { //juste echanger les pos
    for (int i = 0; i < n; i++) {
        int j = rnd(seed,i)* (n - i);
        double tempx = tab[i].xpos;
        double tempy = tab[i].ypos;
        //int temp = tab[i];
        tab[i].xpos = tab[j].xpos;
        tab[i].ypos = tab[j].ypos;
        //tab[i] = tab[j];
        //tab[j] = temp;
        tab[j].xpos = tempx;
        tab[j].ypos = tempy;
    }
    for (int i = 0; i < n; i++) {
        tab[i].ResetSpeed();
    }
    return tab;
}