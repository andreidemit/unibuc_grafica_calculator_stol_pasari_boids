#ifndef PASARE_H
#define PASARE_H

#include <GL/glew.h>

// Structura pentru o pasare (Boids)
struct Pasare {
    GLdouble x, y;           // Pozitia absoluta
    GLdouble vx, vy;         // Viteza
    GLdouble ax, ay;         // Acceleratia (forte boids)
    GLdouble angle;          // Unghiul de orientare
    GLdouble wingPhase;      // Faza animatiei aripilor
    GLdouble wingSpeed;      // Viteza de bataie
    GLdouble size;           // Dimensiunea pasarii
    GLfloat r, g, b;         // Culoarea
};

#endif // PASARE_H
