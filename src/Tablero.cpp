#include "Tablero.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

Tablero::Tablero(int nPuntos): N(nPuntos){
    if(N < 3) N = 3;

    for(int r=0; r<N; ++r){
        for(int c=0; c<N-1; ++c){
            lineas.push_back(Linea(r,c,r,c+1));
        }
    }
    for(int r=0; r<N-1; ++r){
        for(int c=0; c<N; ++c){
            lineas.push_back(Linea(r,c,r+1,c));
        }
    }
    for(int r=0; r<N-1; ++r){
        for(int c=0; c<N-1; ++c){
            casillas.push_back(Casilla(r,c));
        }
    }
}

bool Tablero::adyacentes(int r1,int c1,int r2,int c2){
    int dr = std::abs(r1 - r2);
    int dc = std::abs(c1 - c2);
    return (dr + dc) == 1;
}

void Tablero::canon(int& r1,int& c1,int& r2,int& c2){
    if(r1 > r2 || (r1==r2 && c1 > c2)){
        std::swap(r1,r2);
        std::swap(c1,c2);
    }
}

Tablero::Linea* Tablero::buscarLinea(int r1,int c1,int r2,int c2){
    canon(r1,c1,r2,c2);
    for(std::size_t i=0;i<lineas.size();++i){
        auto &L = lineas.at(i);
        if(L.r1==r1 && L.c1==c1 && L.r2==r2 && L.c2==c2) return &L;
    }
    return nullptr;
}

const Tablero::Linea* Tablero::buscarLinea(int r1,int c1,int r2,int c2) const{
    int a=r1,b=c1,c=r2,d=c2; canon(a,b,c,d);
    for(std::size_t i=0;i<lineas.size();++i){
        const auto &L = lineas.at(i);
        if(L.r1==a && L.c1==b && L.r2==c && L.c2==d) return &L;
    }
    return nullptr;
}

Tablero::Casilla* Tablero::buscarCasilla(int r,int c){
    for(std::size_t i=0;i<casillas.size();++i){
        auto &C = casillas.at(i);
        if(C.r==r && C.c==c) return &C;
    }
    return nullptr;
}

const Tablero::Casilla* Tablero::buscarCasilla(int r,int c) const{
    for(std::size_t i=0;i<casillas.size();++i){
        const auto &C = casillas.at(i);
        if(C.r==r && C.c==c) return &C;
    }
    return nullptr;
}

bool Tablero::casillaCerradaPorLineas(int r,int c) const{
    const Linea* arriba = buscarLinea(r,c,   r,   c+1);
    const Linea* abajo  = buscarLinea(r+1,c, r+1, c+1);
    const Linea* izq    = buscarLinea(r,c,   r+1, c);
    const Linea* der    = buscarLinea(r,c+1, r+1, c+1);
    return (arriba && abajo && izq && der &&
            arriba->dibujada && abajo->dibujada && izq->dibujada && der->dibujada);
}

bool Tablero::evaluarCierreCasilla(int r,int c, char duenoFinal){
    Casilla* cas = buscarCasilla(r,c);
    if(!cas || cas->cerrada) return false;

    if(casillaCerradaPorLineas(r,c)){
        cas->cerrada = true;
        cas->dueno = duenoFinal;
        return true;
    }
    return false;
}

static inline bool casillaTieneUFPara(const Tablero::Linea* a,const Tablero::Linea* b,const Tablero::Linea* c,const Tablero::Linea* d, char closer){
    const Tablero::Linea* lados[4] = {a,b,c,d};
    for(auto li : lados){ if(li && li->ufActiva && li->ufOwner == closer) return true; }
    return false;
}

static inline char resolverACOwner(const Tablero::Linea* a,const Tablero::Linea* b,const Tablero::Linea* c,const Tablero::Linea* d, char defaultOwner){
    const Tablero::Linea* lados[4] = {a,b,c,d};
    for(auto li : lados){ if(li && li->acActiva) return li->acOwner; }
    return defaultOwner;
}

bool Tablero::colocarLinea(int r1,int c1,int r2,int c2, char inicial,
                           int& cerradosTotal, bool& bloqueoEvitoCierre, bool& trampaActivada, char& trampaDuenio,
                           int roundId,
                           bool aplicarTS, char tsVictima,
                           bool aplicarBL, int blRoundActual,
                           bool lsActivo, bool esActivo,
                           bool aplicarUF, bool aplicarAC,
                           int& ptsJugador, int& ptsTrampa){
    cerradosTotal = 0; bloqueoEvitoCierre = false; trampaActivada = false; trampaDuenio = ' ';
    ptsJugador = 0; ptsTrampa = 0;

    if(r1<0||r1>=N||c1<0||c1>=N||r2<0||r2>=N||c2<0||c2>=N) return false;
    if(!adyacentes(r1,c1,r2,c2)) return false;

    Linea* L = buscarLinea(r1,c1,r2,c2);
    if(!L) return false;
    if(L->dibujada) return false;

    L->dibujada = true;

    if(aplicarTS){ L->trapActiva = true; L->trapOwner = inicial; L->trapVictim = tsVictima; L->powerSimbolo = "TS"; }
    if(aplicarBL){ L->bloqueoActivo = true; L->bloqueoOwner = inicial; L->blRoundId = blRoundActual; L->powerSimbolo = "BL"; }
    if(aplicarUF){ L->ufActiva = true; L->ufOwner = inicial; if(L->powerSimbolo.empty()) L->powerSimbolo = "UF"; }
    if(aplicarAC){ L->acActiva = true; L->acOwner = inicial; if(L->powerSimbolo.empty()) L->powerSimbolo = "AC"; }

    auto bloqueoEnCasilla = [&](int rr, int cc)->bool{
        const Linea* arriba = buscarLinea(rr,cc,   rr,   cc+1);
        const Linea* abajo  = buscarLinea(rr+1,cc, rr+1, cc+1);
        const Linea* izq    = buscarLinea(rr,cc,   rr+1, cc);
        const Linea* der    = buscarLinea(rr,cc+1, rr+1, cc+1);
        const Linea* lados[4] = {arriba, abajo, izq, der};
        for(auto li : lados){
            if(li && li->bloqueoActivo && li->blRoundId == roundId && li->bloqueoOwner != inicial){
                return true;
            }
        }
        return false;
    };

    auto desactivarTrampasCasilla = [&](int rr, int cc){
        Linea* arriba = buscarLinea(rr,cc,   rr,   cc+1);
        Linea* abajo  = buscarLinea(rr+1,cc, rr+1, cc+1);
        Linea* izq    = buscarLinea(rr,cc,   rr+1, cc);
        Linea* der    = buscarLinea(rr,cc+1, rr+1, cc+1);
        Linea* lados[4] = {arriba, abajo, izq, der};
        for(auto li : lados){
            if(li && li->trapActiva) li->trapActiva = false;
        }
    };

    auto procesarCasilla = [&](int rr, int cc){
        if(!casillaCerradaPorLineas(rr, cc)) return;

        if(!lsActivo && bloqueoEnCasilla(rr, cc)){
            bloqueoEvitoCierre = true; return;
        }

        const Linea* arriba = buscarLinea(rr,cc,   rr,   cc+1);
        const Linea* abajo  = buscarLinea(rr+1,cc, rr+1, cc+1);
        const Linea* izq    = buscarLinea(rr,cc,   rr+1, cc);
        const Linea* der    = buscarLinea(rr,cc+1, rr+1, cc+1);

        char closer = inicial;
        bool huboTrampa = false;
        if(arriba && arriba->trapActiva && arriba->trapVictim==inicial){ closer = arriba->trapOwner; huboTrampa = true; }
        else if(abajo && abajo->trapActiva && abajo->trapVictim==inicial){ closer = abajo->trapOwner; huboTrampa = true; }
        else if(izq && izq->trapActiva && izq->trapVictim==inicial){ closer = izq->trapOwner; huboTrampa = true; }
        else if(der && der->trapActiva && der->trapVictim==inicial){ closer = der->trapOwner; huboTrampa = true; }

        if(huboTrampa && esActivo){
            closer = inicial;
        }

        char owner = resolverACOwner(arriba, abajo, izq, der, closer);

        if(evaluarCierreCasilla(rr, cc, owner)){
            ++cerradosTotal;

            bool ufDoble = casillaTieneUFPara(arriba, abajo, izq, der, closer);
            int puntos = 1 + (ufDoble ? 1 : 0);

            if(closer == inicial){
                ptsJugador += puntos;
            } else {
                trampaActivada = true; trampaDuenio = closer;
                ptsTrampa += puntos;
            }
            desactivarTrampasCasilla(rr, cc);
        }
    };

    if(r1==r2){
        int r = r1; int cMin = std::min(c1,c2);
        if(r-1 >= 0) procesarCasilla(r-1, cMin);
        if(!bloqueoEvitoCierre && r < N-1) procesarCasilla(r, cMin);
    } else {
        int c = c1; int rMin = std::min(r1,r2);
        if(c-1 >= 0) procesarCasilla(rMin, c-1);
        if(!bloqueoEvitoCierre && c < N-1) procesarCasilla(rMin, c);
    }

    return true;
}

void Tablero::desactivarTrampasContra(char victima){
    for(std::size_t i=0;i<lineas.size();++i){
        auto &L = lineas.at(i);
        if(L.trapActiva && L.trapVictim == victima){
            L.trapActiva = false;
        }
    }
}

int Tablero::contarCuadradosPor(char inicial) const{
    int cnt = 0;
    for(std::size_t i=0;i<casillas.size();++i){
        const auto &C = casillas.at(i);
        if(C.cerrada && C.dueno == inicial) ++cnt;
    }
    return cnt;
}

int Tablero::casillasCerradas() const{
    int cnt = 0;
    for(std::size_t i=0;i<casillas.size();++i){
        if(casillas.at(i).cerrada) ++cnt;
    }
    return cnt;
}

int Tablero::lineasDibujadas() const{
    int cnt = 0;
    for(std::size_t i=0;i<lineas.size();++i){
        if(lineas.at(i).dibujada) ++cnt;
    }
    return cnt;
}

void Tablero::imprimir(bool clarividente, int roundId) const{
    const char* PUNTO = ".";

    auto seg = [&](const Linea* H)->std::string{
        if(!H || !H->dibujada) return "   ";
        if(clarividente && !H->powerSimbolo.empty()){
            if(H->powerSimbolo == "BL" && (!H->bloqueoActivo || H->blRoundId != roundId)){
                return "---";
            }
            std::string s = H->powerSimbolo;
            if(s.size() < 3) s += " ";
            return s.substr(0,3);
        }
        return "---";
    };

    auto col = [&](const Linea* V)->char{
        if(!V || !V->dibujada) return ' ';
        if(clarividente && !V->powerSimbolo.empty()){
            if(V->powerSimbolo == "BL" && (!V->bloqueoActivo || V->blRoundId != roundId)) return '|';
            if(V->powerSimbolo == "TS") return 'T';
            if(V->powerSimbolo == "BL") return 'B';
            if(V->powerSimbolo == "UF") return 'U';
            if(V->powerSimbolo == "AC") return 'A';
        }
        return '|';
    };

    std::cout << "    ";
    for(int c=0; c<N; ++c){
        std::cout << std::setw(4) << c;
    }
    std::cout << "\n";

    for(int r=0; r<N; ++r){
        std::cout << std::setw(3) << r << " ";
        for(int c=0; c<N; ++c){
            std::cout << PUNTO;
            if(c < N-1){
                const Linea* H = buscarLinea(r,c, r, c+1);
                std::cout << seg(H);
            }
        }
        std::cout << "\n";

        if(r < N-1){
            std::cout << "    ";
            for(int c=0; c<N; ++c){
                const Linea* V = buscarLinea(r,c, r+1, c);
                std::cout << col(V);
                if(c < N-1){
                    const Casilla* C = buscarCasilla(r,c);
                    char ch = (C && C->cerrada) ? C->dueno : ' ';
                    std::cout << " " << ch << " ";
                }
            }
            std::cout << "\n";
        }
    }
}
