#include "boids.h"
#include "config.h"
#include "globals.h"
#include <cmath>

// Functie utilitara pentru calculul distantei euclidiene intre doua puncte
double distanta(double x1, double y1, double x2, double y2) {
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// Calculeaza fortele Boids pentru pasarea cu index idx
// Aceasta este inima algoritmului de flocking
void calculeazaForteBoids(int idx) {
    // Initializam vectorii pentru cele 3 reguli principale ale algoritmului Boids
    double sepX = 0, sepY = 0;   // Separare: evita coliziunea cu vecinii
    double alignX = 0, alignY = 0; // Aliniere: zboara in aceeasi directie cu vecinii
    double cohX = 0, cohY = 0;   // Coeziune: stai aproape de centrul grupului
    int sepCount = 0, alignCount = 0, cohCount = 0;
    
    Pasare& p = pasari[idx];
    
    // Parcurgem toate celelalte pasari pentru a vedea care sunt in vecinatate
    for (int j = 0; j < NUM_PASARI; j++) {
        if (j == idx) continue; // Nu ne comparam cu noi insine
        
        // Distanta fata de pasarea j
        double dist = distanta(p.x, p.y, pasari[j].x, pasari[j].y);
        
        // REGULA 1: SEPARARE - evita vecinii prea apropiati
        if (dist < SEPARATION_RADIUS && dist > 0) {
            double dx = p.x - pasari[j].x;
            double dy = p.y - pasari[j].y;
            // Forta inversa proportionala cu distanta (cu cat e mai aproape, cu atat respingerea e mai puternica)
            sepX += dx / dist;
            sepY += dy / dist;
            sepCount++; // Numara vecinii apropiati
        }
        
        // REGULA 2: ALINIERE - potriveste viteza cu vecinii din raza vizuala
        if (dist < ALIGNMENT_RADIUS) {
            alignX += pasari[j].vx;
            alignY += pasari[j].vy;
            alignCount++; // Numara vecinii din raza de aliniere
        }
        
        // REGULA 3: COEZIUNE - indreapta-te spre centrul de masa al vecinilor
        if (dist < COHESION_RADIUS) {
            cohX += pasari[j].x;
            cohY += pasari[j].y;
            cohCount++; // Numara vecinii din raza de coeziune
        }
    }
    
    // Reseteaza acceleratia curenta inainte de a aduna noile forte
    p.ax = 0;
    p.ay = 0;
    
    // Aplica forta de SEPARARE (media vectorilor de respingere)
    if (sepCount > 0) {
        sepX /= sepCount;
        sepY /= sepCount;
        double sepMag = sqrt(sepX*sepX + sepY*sepY);
        if (sepMag > 0) {
            p.ax += (sepX / sepMag) * SEPARATION_WEIGHT;
            p.ay += (sepY / sepMag) * SEPARATION_WEIGHT;
        }
    }
    
    // Aplica forta de ALINIERE (media vitezelor vecinilor)
    if (alignCount > 0) {
        alignX /= alignCount;
        alignY /= alignCount;
        // Steering (Directie) = Desired (Dorita) - Current (Curenta)
        double steerX = alignX - p.vx;
        double steerY = alignY - p.vy;
        double steerMag = sqrt(steerX*steerX + steerY*steerY);
        if (steerMag > 0) {
            p.ax += (steerX / steerMag) * ALIGNMENT_WEIGHT * 0.1;
            p.ay += (steerY / steerMag) * ALIGNMENT_WEIGHT * 0.1;
        }
    }
    
    // Aplica forta de COEZIUNE (atractia catre centrul de masa al vecinilor)
    if (cohCount > 0) {
        cohX /= cohCount;  // Calculam centrul de masa (media pozitiilor)
        cohY /= cohCount;
        double dx = cohX - p.x;
        double dy = cohY - p.y;
        double dist = sqrt(dx*dx + dy*dy);
        if (dist > 0) {
            p.ax += (dx / dist) * COHESION_WEIGHT * 0.05;
            p.ay += (dy / dist) * COHESION_WEIGHT * 0.05;
        }
    }
    
    // REGULA SUPLIMENTARA: EVITARE MARGINI
    // Daca pasarea se apropie de marginea ferestrei, aplicam o forta opusa
    if (p.x < EDGE_MARGIN) {
        p.ax += EDGE_FORCE * (EDGE_MARGIN - p.x) / EDGE_MARGIN;
    } else if (p.x > WINDOW_WIDTH - EDGE_MARGIN) {
        p.ax -= EDGE_FORCE * (p.x - (WINDOW_WIDTH - EDGE_MARGIN)) / EDGE_MARGIN;
    }
    if (p.y < EDGE_MARGIN) {
        p.ay += EDGE_FORCE * (EDGE_MARGIN - p.y) / EDGE_MARGIN;
    } else if (p.y > WINDOW_HEIGHT - EDGE_MARGIN) {
        p.ay -= EDGE_FORCE * (p.y - (WINDOW_HEIGHT - EDGE_MARGIN)) / EDGE_MARGIN;
    }
    
    // REGULA SUPLIMENTARA: ATRACTIE MOUSE (slaba)
    double dx = targetX - p.x;
    double dy = targetY - p.y;
    double dist = sqrt(dx*dx + dy*dy);
    if (dist > 0) {
        p.ax += (dx / dist) * MOUSE_ATTRACTION;
        p.ay += (dy / dist) * MOUSE_ATTRACTION;
    }
}

// Functia principala de actualizare a starii simularii (fizica)
void actualizeaza() {
    // Actualizeaza fiecare pasare folosind algoritmul Boids
    for (int i = 0; i < NUM_PASARI; i++) {
        // 1. Calculeaza toate fortele care actioneaza asupra pasarii (Boids + Mediu)
        calculeazaForteBoids(i);
        
        // 2. Fizica Newtoniana: Viteza = Viteza + Acceleratia
        pasari[i].vx += pasari[i].ax;
        pasari[i].vy += pasari[i].ay;
        
        // 3. Aplica frecarea cu aerul (Drag) pentru a preveni accelerarea infinita
        pasari[i].vx *= DRAG;
        pasari[i].vy *= DRAG;
        
        // 4. Limiteaza viteza maxima (pentru realism si stabilitate)
        double speed = sqrt(pasari[i].vx * pasari[i].vx + pasari[i].vy * pasari[i].vy);
        if (speed > MAX_SPEED) {
            pasari[i].vx = (pasari[i].vx / speed) * MAX_SPEED;
            pasari[i].vy = (pasari[i].vy / speed) * MAX_SPEED;
            speed = MAX_SPEED;
        }
        
        // 5. Actualizeaza pozitia: Pozitie = Pozitie + Viteza
        pasari[i].x += pasari[i].vx;
        pasari[i].y += pasari[i].vy;
        
        // 6. Calculam unghiul de rotatie pentru desenare
        // Pasarea se roteste spre directia in care se misca (cu interpolare linara)
        if (speed > 0.1) {
            double targetAngle = atan2(pasari[i].vy, pasari[i].vx);
            double angleDiff = targetAngle - pasari[i].angle; // Diferenta unghiulara dintre directia dorita si cea curenta
            while (angleDiff > PI) angleDiff -= 2 * PI; // Normalizeaza intre -PI si PI
            while (angleDiff < -PI) angleDiff += 2 * PI; // Normalizeaza intre -PI si PI
            pasari[i].angle += angleDiff * 0.15;  // Rotire linara spre tinta (15% din diferenta pe frame, pentru netezime)
        }
        
        // 7. Animeaza aripile - frecventa depinde de viteza de deplasare
        double speedRatio = speed / MAX_SPEED;
        pasari[i].wingPhase += pasari[i].wingSpeed * (0.5 + 0.5 * speedRatio); // Mai repede cand zboara mai repede
        if (pasari[i].wingPhase > 2 * PI) pasari[i].wingPhase -= 2 * PI; 
    }
}
