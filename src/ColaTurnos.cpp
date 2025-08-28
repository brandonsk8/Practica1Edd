#include "ColaTurnos.h"
#include <stdexcept>

ColaTurnos::~ColaTurnos(){ clear(); }

void ColaTurnos::enqueue(Jugador* j){
    Nodo* n = new Nodo(j);
    if(!fondo){
        frente = fondo = n;
    } else {
        fondo->siguiente = n;
        fondo = n;
    }
    ++elementos;
}

Jugador* ColaTurnos::front() const{
    if(!frente) throw std::runtime_error("Cola vacia");
    return frente->jugador;
}

Jugador* ColaTurnos::dequeue(){
    if(!frente) throw std::runtime_error("Cola vacia");
    Nodo* n = frente;
    Jugador* j = n->jugador;
    frente = frente->siguiente;
    if(!frente) fondo = nullptr;
    delete n;
    --elementos;
    return j;
}

bool ColaTurnos::moveToFront(Jugador* j){
    if(!frente || !frente->siguiente) return false; // 0 o 1 elemento
    if(frente->jugador == j) return true; // ya está al frente

    Nodo* prev = frente;
    Nodo* curr = frente->siguiente;
    while(curr){
        if(curr->jugador == j){
            prev->siguiente = curr->siguiente;
            if(curr == fondo) fondo = prev;
            curr->siguiente = frente;
            frente = curr;
            return true;
        }
        prev = curr;
        curr = curr->siguiente;
    }
    return false; // no encontrado
}

Jugador* ColaTurnos::peekNext() const{
    if(!frente) return nullptr;
    return frente->siguiente ? frente->siguiente->jugador : frente->jugador;
}

void ColaTurnos::clear(){
    while(frente){
        Nodo* n = frente;
        frente = frente->siguiente;
        delete n;
    }
    fondo = nullptr;
    elementos = 0;
}
