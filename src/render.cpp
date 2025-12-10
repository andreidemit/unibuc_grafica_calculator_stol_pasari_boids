// Acest fisier gestioneaza randarea grafica a simularii folosind OpenGL.
// Contine functii pentru desenarea elementelor individuale (pasari) si a scenei complete.

#include "render.h"
#include "config.h"     // Constante pentru configurarea vizuala
#include "globals.h"    // Acces la datele simularii (pozitii pasari)
#include <GL/glew.h>    // Acces la functiile OpenGL
#include <cmath>

// Functie pentru desenarea unei singure pasari
// Foloseste transformari geometrice (translatie, rotatie, scalare)
void desenarePasare(Pasare& p) {
    // Izolam transformarile geometrice pentru aceasta pasare folosind stiva de matrici.
    // Orice modificare (rotatie, translatie) facuta aici nu va afecta restul scenei.
    glPushMatrix(); 
    
    // 1. Pozitionare: Mutam sistemul de coordonate la pozitia pasarii (x, y)
    glTranslated(p.x, p.y, 0.0);
    
    // 2. Orientare: Rotim pasarea in functie de unghiul ei de deplasare
    // Convertim din radiani in grade pentru OpenGL
    glRotated(p.angle * 180.0 / PI, 0.0, 0.0, 1.0);
    
    // Corectie de orientare: Rotim cu -90 grade pentru ca modelul nostru (triunghiul)
    // este desenat "in sus" (axa Y), dar 0 grade matematic este "la dreapta" (axa X)
    glRotated(-90.0, 0.0, 0.0, 1.0); 
    
    // 3. Scalare: Ajustam dimensiunea pasarii
    glScaled(p.size, p.size, 1.0);
    
    // Simulare simpla a bataii aripilor prin modificarea latimii triunghiului
    // Folosim functia sin() pentru o miscare oscilatorie lina (valori intre -1 si 1)
    double wingWidth = 8.0 + sin(p.wingPhase) * 2.0;

    
    // Desenam corpul pasarii folosind primitive grafice (triunghiuri)
    glColor3f(p.r, p.g, p.b);
    glBegin(GL_TRIANGLES);
    
    // Jumatatea stanga a pasarii
    // Varful din fata (capul)
    glVertex2f(0, 15);
    // Colt stanga spate (varful aripii stangi)
    glVertex2f(-wingWidth, -10);
    // Centru spate (coada - scobitura pentru aspect de sageata)
    glVertex2f(0, -5);
    
    // Jumatatea dreapta a pasarii
    // Varful din fata (repetat)
    glVertex2f(0, 15);
    // Centru spate
    glVertex2f(0, -5);
    // Colt dreapta spate (varful aripii drepte)
    glVertex2f(wingWidth, -10);
    glEnd();
    
    // Adaugam o linie mediana pentru un detaliu vizual subtil (coloana vertebrala)
    // Culoarea este putin mai intunecata decat corpul
    glColor3f(p.r - 0.2f, p.g - 0.2f, p.b - 0.2f);
    glBegin(GL_LINES);
    glVertex2f(0, 15);
    glVertex2f(0, -5);
    glEnd();

    // Debug: Desenam un cerc in jurul pasarii pentru a vizualiza zona de influenta
    // glColor3f(1.0, 1.0, 1.0); // Alb
    // glBegin(GL_LINE_LOOP);
    // for(int i = 0; i< 12; i++){
    //     float angle = 2.0f * PI * i / 12;
    //     glVertex2f(cos(angle) * 15, sin(angle) * 15); // 15 este raza cercului
    // }
    // glEnd();
    
    // Revenim la sistemul de coordonate anterior (cel global), anuland transformarile locale
    glPopMatrix(); 
}

// Functia principala de randare a scenei
void desenareStol() {
    // Setam modul de lucru cu matricea Model-View (pentru transformari de obiecte si camera)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Resetam transformarile
    
    // Desenare fundal (Nori) - elemente decorative statice
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // Alb semi-transparent
    for (int n = 0; n < 4; n++) {
        double nx = 80 + n * 200;
        double ny = 480 + (n % 2) * 60; 
        // Un nor este compus din 3 cercuri suprapuse
        for (int c = 0; c < 3; c++) {
            glBegin(GL_POLYGON);
            // Generam varfurile cercului folosind ecuatia parametrica:
            // x = x_centru + raza * cos(unghi)
            // y = y_centru + raza * sin(unghi)
            for (int i = 0; i < 16; i++) {
                float a = 2.0f * PI * i / 16;
                glVertex2f(nx + c*30 + cos(a) * 35, ny + sin(a) * 20);
            }
            glEnd();
        }
    }
    
    // Deseneaza toate pasarile din stol
    // Le parcurgem invers (de la ultima la prima) daca vrem un anumit Z-order,
    // desi in 2D simplu ordinea conteaza mai putin decat in 3D
    for (int i = NUM_PASARI - 1; i >= 0; i--) {
        desenarePasare(pasari[i]);
    }
    
    glFlush(); // Fortam executia comenzilor OpenGL
}
