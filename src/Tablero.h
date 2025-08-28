#ifndef TABLERO_H
#define TABLERO_H

#include <string>
#include "ListaEnlazada.h"

class Tablero {
public:
    struct Linea {
        int r1, c1, r2, c2;
        bool dibujada {false};
        std::string powerSimbolo;

        bool trapActiva {false};
        char trapOwner {' '};
        char trapVictim {' '};

        bool bloqueoActivo {false};
        char bloqueoOwner {' '};
        int blRoundId {-1};

        bool ufActiva {false};
        char ufOwner {' '};

        bool acActiva {false};
        char acOwner {' '};

        Linea(int R1,int C1,int R2,int C2): r1(R1),c1(C1),r2(R2),c2(C2) {}
    };

    struct Casilla {
        int r, c;
        bool cerrada {false};
        char dueno {' '};
        Casilla(int R,int C): r(R),c(C) {}
    };

private:
    int N;
    ListaEnlazada<Linea>   lineas;
    ListaEnlazada<Casilla> casillas;

public:
    explicit Tablero(int nPuntos);

    // UNICA declaracion valida
    bool colocarLinea(int r1,int c1,int r2,int c2, char inicial,
                      int& cerradosTotal, bool& bloqueoEvitoCierre, bool& trampaActivada, char& trampaDuenio,
                      int roundId,
                      bool aplicarTS, char tsVictima,
                      bool aplicarBL, int blRoundActual,
                      bool lsActivo, bool esActivo,
                      bool aplicarUF, bool aplicarAC,
                      int& ptsJugador, int& ptsTrampa);

    void desactivarTrampasContra(char victima);
    void imprimir(bool clarividente=false, int roundId=0) const;

    int puntosPorLado() const { return N; }
    int casillasTotales() const { return (N-1)*(N-1); }
    int casillasCerradas() const;
    int totalLineas() const { return 2 * N * (N - 1); }
    int lineasDibujadas() const;
    int lineasRestantes() const { return totalLineas() - lineasDibujadas(); }
    bool finDePartida() const { return casillasCerradas() == casillasTotales() || lineasRestantes() == 0; }

    int contarCuadradosPor(char inicial) const;

private:
    static bool adyacentes(int r1,int c1,int r2,int c2);
    static void canon(int& r1,int& c1,int& r2,int& c2);

    Linea*       buscarLinea(int r1,int c1,int r2,int c2);
    const Linea* buscarLinea(int r1,int c1,int r2,int c2) const;

    Casilla*       buscarCasilla(int r,int c);
    const Casilla* buscarCasilla(int r,int c) const;

    bool evaluarCierreCasilla(int r,int c, char duenoFinal);
    bool casillaCerradaPorLineas(int r,int c) const;
};

#endif // TABLERO_H
