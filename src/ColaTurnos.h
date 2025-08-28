#ifndef COLA_TURNOS_H
#define COLA_TURNOS_H

#include <cstddef>
#include "Jugador.h"

// Cola enlazada de punteros a Jugador (no gestiona la vida de los jugadores)
class ColaTurnos {
private:
    struct Nodo {
        Jugador* jugador {nullptr};
        Nodo* siguiente {nullptr};
        explicit Nodo(Jugador* j): jugador(j) {}
    };

    Nodo* frente {nullptr};
    Nodo* fondo {nullptr};
    std::size_t elementos {0};

public:
    ColaTurnos() = default;
    ~ColaTurnos();

    bool empty() const { return elementos == 0; }
    std::size_t size() const { return elementos; }

    void enqueue(Jugador* j);
    Jugador* front() const; // no extrae
    Jugador* dequeue();     // extrae

    // Utilidad requerida por reglas  mover un jugador al frente
    bool moveToFront(Jugador* j);

    // Ver el siguiente jugador sin modificar la cola
    Jugador* peekNext() const;

    void clear();
};

#endif // COLA_TURNOS_H
