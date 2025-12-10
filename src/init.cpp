#include "init.h"
#include "config.h"
#include "globals.h"
#include <GL/glew.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Functie pentru initializarea starii fiecarei pasari la inceputul simularii
void initPasari() {
    // Initializam generatorul de numere aleatorii bazat pe timp
    srand(time(NULL));
    
    // Plaseaza pasarile aleatoriu in zona centrala
    for (int i = 0; i < NUM_PASARI; i++) {
        // Pozitie aleatorie in zona centrala (200-600, 150-450)
        // Evitam marginile pentru a nu declansa imediat fortele de evitare
        pasari[i].x = 200.0 + rand() % 400;
        pasari[i].y = 150.0 + rand() % 300;
        
        // Viteza initiala aleatorie (in general spre dreapta)
        // Le dam un impuls initial pentru a porni miscarea
        double angle = ((rand() % 100) / 100.0 - 0.5) * PI * 0.5;  // -45 to +45 grade
        double speed = 1.5 + (rand() % 100) / 50.0;
        pasari[i].vx = cos(angle) * speed;
        pasari[i].vy = sin(angle) * speed;
        pasari[i].ax = 0;
        pasari[i].ay = 0;
        pasari[i].angle = angle;
        
        // Animatie aripi cu faze diferite
        // "Phase" asigura ca nu toate pasarile bat din aripi sincronizat (ar arata artificial)
        pasari[i].wingPhase = (rand() % 100) / 100.0 * 2 * PI;
        pasari[i].wingSpeed = 0.15 + (rand() % 30) / 300.0;
        
        // Dimensiune variata pentru realism
        pasari[i].size = 0.8 + (rand() % 30) / 100.0;
        
        // Culori variate (nuante de gri-maro pentru pasari)
        // Variatia subtila ajuta la distingerea indivizilor
        float base = 0.15f + (rand() % 25) / 100.0f;
        pasari[i].r = base + 0.05f;
        pasari[i].g = base;
        pasari[i].b = base + 0.03f;
    }
}

// Functia de initializare generala a aplicatiei OpenGL
void init() {
    // Setam culoarea de fundal (cer senin)
    glClearColor(0.5f, 0.75f, 0.95f, 0.0f);
    
    // Configuram proiectia ortografica 2D
    // Aceasta mapeaza coordonatele logice (0..WINDOW_WIDTH, 0..WINDOW_HEIGHT) direct pe ecran
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT, -1.0, 1.0);
    
    // Revenim la matricea ModelView pentru desenarea obiectelor
    glMatrixMode(GL_MODELVIEW);
    
    // Activam amestecarea culorilor (Blending) pentru transparenta (folosita la nori)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Activam antialiasing pentru linii mai fine
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    // Initializam datele simularii
    initPasari();
}
