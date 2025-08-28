#ifndef PILA_POWERUPS_H
#define PILA_POWERUPS_H

#include "PowerUp.h"
#include <stdexcept>

class PilaPowerUps {
    struct Nodo {
        PowerUp val;
        Nodo* sig;
        explicit Nodo(const PowerUp& p): val(p), sig(nullptr) {}
    };
    Nodo* tope {nullptr};
    std::size_t n {0};

public:
    PilaPowerUps() = default;
    ~PilaPowerUps(){ clear(); }

    bool empty() const { return n == 0; }
    std::size_t size() const { return n; }

    void push(const PowerUp& p){
        Nodo* nuevo = new Nodo(p);
        nuevo->sig = tope;
        tope = nuevo;
        ++n;
    }

    PowerUp pop(){
        if(empty()) throw std::runtime_error("Pila vacia");
        Nodo* t = tope;
        PowerUp v = t->val;
        tope = tope->sig;
        delete t;
        --n;
        return v;
    }

    PowerUp top() const {
        if(empty()) throw std::runtime_error("Pila vacia");
        return tope->val;
    }

    void clear(){
        while(tope){
            Nodo* t = tope;
            tope = tope->sig;
            delete t;
        }
        n = 0;
    }
};

#endif // PILA_POWERUPS_H
