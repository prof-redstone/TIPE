
#include <iostream> //pour ecrire dans la console.
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
    unsigned int nSeed = seed*5323 + index*8647;
    nSeed = (8253729 * nSeed + 2396403);
    return nSeed;
}

double rnd(int seed, int index) {
    int res = PRNG(seed, index) % 10000;
    return ((double)res) / 10000;
}

vector <Boule> shuffle(vector <Boule> tab, int n,int seed) { //juste echanger les pos
    for (int i = 0; i < n; i++) {
        int j = (int)((rnd(seed, i)+1)/2 * (i));
        double tempx = tab[i].xpos;
        double tempy = tab[i].ypos;
        tab[i].xpos = tab[j].xpos;
        tab[i].ypos = tab[j].ypos;
        tab[j].xpos = tempx;
        tab[j].ypos = tempy;
    }
    for (int i = 0; i < n; i++) {
        tab[i].ResetSpeed();
    }
    return tab;
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float perlin_noise(int seed, float x) {
    // Prépare les variables
    const float zoom = 4.0;
    float u = x * zoom;
    int i = int(u);
    float f = u - i;

    // Génère les valeurs aléatoires
    float v1 = rnd(seed, i);
    float v2 = rnd(seed, i + 1);

    // Calcule le bruit
    return lerp(v1, v2, f);
}