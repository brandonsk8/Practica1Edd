#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "PilaPowerUps.h"

class Jugador {
private:
    std::string nombre;
    char inicial {'?'};
    int puntos {0};
    int cuadradosCerrados {0}; // (informativo; al final contamos en Tablero)
    PilaPowerUps poderes;

public:
    Jugador() = default;
    Jugador(std::string nombre_, char inicial_): nombre(std::move(nombre_)), inicial(inicial_) {}

    const std::string& getNombre() const { return nombre; }
    char getInicial() const { return inicial; }
    int getPuntos() const { return puntos; }
    int getCuadradosCerrados() const { return cuadradosCerrados; }

    void sumarPunto(int n = 1) { puntos += n; }
    void sumarCuadrado(int n = 1) { cuadradosCerrados += n; }

    // Pila de poderes
    void agregarPowerUp(const PowerUp& p) { poderes.push(p); }
    bool tienePowerUps() const { return !poderes.empty(); }
    PowerUp verPowerUpTope() const { return poderes.top(); }
    PowerUp usarPowerUp();

};

#endif // JUGADOR_H
