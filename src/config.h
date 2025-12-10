#ifndef CONFIG_H
#define CONFIG_H

// Numar de pasari in stol
const int NUM_PASARI = 20;

// Constante matematice
const double PI = 3.14159265358979;

// Parametri Boids
const double MAX_SPEED = 4.0;
const double SEPARATION_RADIUS = 25.0;
const double SEPARATION_WEIGHT = 1.8;
const double ALIGNMENT_RADIUS = 50.0;
const double ALIGNMENT_WEIGHT = 1.0;
const double COHESION_RADIUS = 80.0;
const double COHESION_WEIGHT = 1.0;
const double EDGE_MARGIN = 60.0;
const double EDGE_FORCE = 0.3;
const double MOUSE_ATTRACTION = 0.015;
const double DRAG = 0.97;

// Dimensiuni fereastra
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

#endif // CONFIG_H
