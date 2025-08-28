#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include <cstddef>
#include <stdexcept>

template<typename T>
class ListaEnlazada {
    struct Nodo {
        T dato;
        Nodo* sig;
        explicit Nodo(const T& d): dato(d), sig(nullptr) {}
    };
    Nodo* cabeza {nullptr};
    Nodo* cola {nullptr};
    std::size_t n {0};

public:
    ListaEnlazada() = default;
    ~ListaEnlazada() { clear(); }

    void push_back(const T& val){
        Nodo* nuevo = new Nodo(val);
        if(!cola){ cabeza = cola = nuevo; }
        else { cola->sig = nuevo; cola = nuevo; }
        ++n;
    }

    std::size_t size() const { return n; }

    T& at(std::size_t idx){
        if(idx >= n) throw std::out_of_range("ListaEnlazada::at");
        Nodo* cur = cabeza;
        while(idx--) cur = cur->sig;
        return cur->dato;
    }
    const T& at(std::size_t idx) const {
        if(idx >= n) throw std::out_of_range("ListaEnlazada::at const");
        Nodo* cur = cabeza;
        std::size_t i = idx;
        while(i--) cur = cur->sig;
        return cur->dato;
    }

    void clear(){
        while(cabeza){
            Nodo* t = cabeza;
            cabeza = cabeza->sig;
            delete t;
        }
        cola = nullptr;
        n = 0;
    }
};

#endif // LISTA_ENLAZADA_H
