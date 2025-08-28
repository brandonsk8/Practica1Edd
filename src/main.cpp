#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "Jugador.h"
#include "ColaTurnos.h"
#include "PowerUp.h"
#include "Tablero.h"

static char asignarInicialAuto(int idx){
    return static_cast<char>('A' + (idx % 26));
}

static void mostrarAyuda(){
    std::cout
      << "\nComandos:\n"
      << "  r1 c1 r2 c2  -> trazar linea entre puntos adyacentes (0-based)\n"
      << "  u            -> usar poder del tope (DL/PS/TS/BL/LS/ES/UF/AC)\n"
      << "  p            -> atajo para PS (pase)\n"
      << "  v            -> alternar modo clarividente (muestra TS/BL/UF/AC)\n"
      << "  h            -> ayuda\n"
      << "  -1           -> salir\n\n";
}

int main(){
    std::cout << "=== Totito Chino - Practica 1 EDD ===\n";
    std::cout << "*** Build: " << __DATE__ << " " << __TIME__ << " ***\n";

    int nPuntos = 3;
    int jugadoresN = 0;

    std::cout << "Tamano del tablero (puntos por lado, >=3): ";
    std::cin >> nPuntos;
    if(!std::cin){ return 0; }
    if(nPuntos < 3){
        std::cout << "Valor invalido, se usara 3.\n";
        nPuntos = 3;
    }

    std::cout << "Numero de jugadores: ";
    std::cin >> jugadoresN;
    if(!std::cin || jugadoresN < 1){
        std::cout << "Se necesita al menos 1 jugador. Saliendo.\n";
        return 0;
    }

    std::vector<Jugador> jugadores;
    jugadores.reserve(static_cast<std::size_t>(jugadoresN));

    for(int i=0;i<jugadoresN;++i){
        std::string nombre;
        std::cout << "Nombre del jugador " << (i+1) << ": ";
        std::cin >> nombre;

        char inicial;
        std::cout << "Inicial (1 char) o '*' para automatica: ";
        std::cin >> inicial;
        if(inicial == '*') inicial = asignarInicialAuto(i);

        jugadores.emplace_back(nombre, inicial);
    }

    ColaTurnos cola;
    for(auto& j : jugadores) cola.enqueue(&j);

    // asignar algunos poderes de prueba
    for(std::size_t i=0;i<jugadores.size();++i){
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::DL));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::PS));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::TS));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::BL));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::LS));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::ES));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::UF));
        jugadores[i].agregarPowerUp(PowerUp(PowerUpType::AC));
    }

    Tablero board(nPuntos);
    bool clarividente = false;

    Jugador* jugadorInicioRonda = cola.front();
    int roundId = 0;

    bool pendingTS = false;      char pendingTSVictim = ' ';
    bool pendingBL = false;      int  pendingBLRound = -1;
    bool pendingLS = false;
    bool pendingES = false;
    bool pendingUF = false;
    bool pendingAC = false;

    auto check_end = [&](){
        if(board.finDePartida()){
            std::cout << "\n>> No quedan mas movimientos. Fin de la partida.\n";
            return true;
        }
        return false;
    };

    std::cout << "\nTablero inicial:\n";
    board.imprimir(clarividente, roundId);
    mostrarAyuda();

    int dlRestantes = 0;
    bool gameover = false;

    while(!gameover){
        Jugador* actual = cola.front();

        std::cout << "Turno de " << actual->getNombre() << " [" << actual->getInicial() << "]";
        if(actual->tienePowerUps()){
            try{
                PowerUp top = actual->verPowerUpTope();
                std::cout << "  (tope PU: " << top.simbolo() << ")";
            } catch(...){}
        }
        if(dlRestantes>0) std::cout << "  <DL:" << dlRestantes << ">";
        if(pendingTS) std::cout << "  <TS->" << pendingTSVictim << ">";
        if(pendingBL) std::cout << "  <BL:ronda>";
        if(pendingLS) std::cout << "  <LS>";
        if(pendingES) std::cout << "  <ES>";
        if(pendingUF) std::cout << "  <UF>";
        if(pendingAC) std::cout << "  <AC>";
        std::cout << "\n> ";

        std::string tok; if(!(std::cin >> tok)) break;
        if(tok == "-1") break;

        if(tok == "h"){ mostrarAyuda(); continue; }

        if(tok == "v"){
            clarividente = !clarividente;
            board.imprimir(clarividente, roundId);
            if(check_end()){ gameover = true; }
            continue;
        }

        if(tok == "u"){
            if(!actual->tienePowerUps()){ std::cout << "No tienes powerups.\n"; continue; }
            PowerUp top = actual->verPowerUpTope();
            if(top.tipo == PowerUpType::DL){
                (void)actual->usarPowerUp();
                dlRestantes = 2;
                std::cout << "DL activado: puedes trazar 2 lineas seguidas.\n";
            } else if(top.tipo == PowerUpType::PS){
                (void)actual->usarPowerUp();
                cola.dequeue(); cola.enqueue(actual);
                if(cola.front() == jugadorInicioRonda){ ++roundId; }
                jugadorInicioRonda = cola.front();
                dlRestantes = 0; pendingTS = pendingBL = pendingLS = pendingES = pendingUF = pendingAC = false;
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
            } else if(top.tipo == PowerUpType::TS){
                (void)actual->usarPowerUp();
                pendingTS = true; {
                    Jugador* nxt = cola.peekNext();
                    pendingTSVictim = nxt ? nxt->getInicial() : actual->getInicial();
                }
                std::cout << "TS listo: la PROXIMA linea que traces quedara con trampa contra '" << pendingTSVictim << "'.\n";
            } else if(top.tipo == PowerUpType::BL){
                (void)actual->usarPowerUp();
                pendingBL = true; pendingBLRound = roundId;
                std::cout << "BL listo: la PROXIMA linea que traces quedara bloqueada hasta fin de ronda.\n";
            } else if(top.tipo == PowerUpType::LS){
                (void)actual->usarPowerUp();
                pendingLS = true; std::cout << "LS activada para tu PROXIMA linea.\n";
            } else if(top.tipo == PowerUpType::ES){
                (void)actual->usarPowerUp();
                pendingES = true; std::cout << "ES activado para tu PROXIMA linea.\n";
            } else if(top.tipo == PowerUpType::UF){
                (void)actual->usarPowerUp();
                pendingUF = true; std::cout << "UF listo: si TU cierras mas adelante usando esa linea, valdra 2 puntos.\n";
            } else if(top.tipo == PowerUpType::AC){
                (void)actual->usarPowerUp();
                pendingAC = true; std::cout << "AC listo: la casilla que se cierre con esa linea quedara como tu propiedad.\n";
            } else {
                std::cout << "PowerUp no implementado.\n";
            }
            continue;
        }

        if(tok == "p"){
            if(actual->tienePowerUps()){
                PowerUp top = actual->verPowerUpTope();
                if(top.tipo == PowerUpType::PS){
                    (void)actual->usarPowerUp();
                    cola.dequeue(); cola.enqueue(actual);
                    if(cola.front() == jugadorInicioRonda){ ++roundId; }
                    jugadorInicioRonda = cola.front();
                    dlRestantes = 0; pendingTS = pendingBL = pendingLS = pendingES = pendingUF = pendingAC = false;
                    board.imprimir(clarividente, roundId);
                    if(check_end()){ gameover = true; }
                } else {
                    std::cout << "Tu poder tope no es PS. Usa 'u' para otros.\n";
                }
            } else {
                std::cout << "No tienes powerups.\n";
            }
            continue;
        }

        int r1,c1,r2,c2;
        if(tok == "l"){
            if(!(std::cin >> r1 >> c1 >> r2 >> c2)) break;
        } else {
            try { r1 = std::stoi(tok); }
            catch(...) { std::cout << "Entrada invalida. Usa: r1 c1 r2 c2 | u | p | v | h | -1\n"; continue; }
            if(!(std::cin >> c1 >> r2 >> c2)) break;
        }

        int cerrados = 0; bool bloqueoTurno=false; bool trampAct=false; char trampOwner=' ';
        int ptsJugador=0, ptsTrampa=0;
        if(board.colocarLinea(r1,c1,r2,c2, actual->getInicial(), cerrados, bloqueoTurno, trampAct, trampOwner,
                              roundId, pendingTS, pendingTSVictim, pendingBL, pendingBLRound,
                              pendingLS, pendingES, pendingUF, pendingAC,
                              ptsJugador, ptsTrampa)){
            pendingTS = false; pendingBL = false; pendingBLRound = -1; pendingLS = false; pendingES = false; pendingUF = false; pendingAC = false;

            if(ptsJugador > 0){ actual->sumarPunto(ptsJugador); }
            if(ptsTrampa  > 0){ for(auto &j : jugadores){ if(j.getInicial()==trampOwner){ j.sumarPunto(ptsTrampa); break; } } }

            if(trampAct){
                for(auto &j : jugadores){ if(j.getInicial()==trampOwner){ cola.moveToFront(&j); break; } }
                dlRestantes = 0;
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
                continue;
            }

            if(bloqueoTurno){
                std::cout << "Intentaste cerrar usando una linea BLOQUEADA. Pierdes el turno.\n";
                dlRestantes = 0;
                board.desactivarTrampasContra(actual->getInicial());
                cola.dequeue(); cola.enqueue(actual);
                if(cola.front() == jugadorInicioRonda){ ++roundId; }
                jugadorInicioRonda = cola.front();
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
                continue;
            }

            if(ptsJugador > 0){
                if(dlRestantes > 0) dlRestantes = 0;
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
                continue;
            }

            if(dlRestantes > 0){
                --dlRestantes;
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
                if(dlRestantes > 0){
                    std::cout << "Te queda 1 linea del DL.\n";
                    continue;
                }
                dlRestantes = 0;
                board.desactivarTrampasContra(actual->getInicial());
                cola.dequeue(); cola.enqueue(actual);
                if(cola.front() == jugadorInicioRonda){ ++roundId; }
                jugadorInicioRonda = cola.front();
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
            } else {
                board.desactivarTrampasContra(actual->getInicial());
                cola.dequeue(); cola.enqueue(actual);
                if(cola.front() == jugadorInicioRonda){ ++roundId; }
                jugadorInicioRonda = cola.front();
                board.imprimir(clarividente, roundId);
                if(check_end()){ gameover = true; }
            }
        } else {
            std::cout << "\nMovimiento incorrecto. Intenta de nuevo (puntos adyacentes y linea no dibujada).\n";
            if(!std::cin){ std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
        }
    }

    std::cout << "\nPuntajes finales (puntos / cuadrados):\n";
    int maxPts = -1;
    for(const auto& j : jugadores){
        int cuadrados = board.contarCuadradosPor(j.getInicial());
        std::cout << " - " << j.getNombre() << " [" << j.getInicial() << "] -> "
                  << j.getPuntos() << " / " << cuadrados << "\n";
        if(j.getPuntos() > maxPts) maxPts = j.getPuntos();
    }
    std::vector<int> idx;
    for(int i=0;i<(int)jugadores.size();++i)
        if(jugadores[i].getPuntos() == maxPts) idx.push_back(i);

    if(idx.size() == 1){
        std::cout << "\nGANADOR: " << jugadores[idx[0]].getNombre() << " [" << jugadores[idx[0]].getInicial() << "]\n";
    } else {
        int bestSq = -1;
        std::vector<int> idx2;
        for(int i: idx){
            int sq = board.contarCuadradosPor(jugadores[i].getInicial());
            if(sq > bestSq){ bestSq = sq; idx2.clear(); idx2.push_back(i); }
            else if(sq == bestSq){ idx2.push_back(i); }
        }
        if(idx2.size() == 1){
            int w = idx2[0];
            std::cout << "\nGANADOR (desempate por cuadrados): "
                      << jugadores[w].getNombre() << " [" << jugadores[w].getInicial() << "]\n";
        } else {
            std::cout << "\nEMPATE entre:";
            for(int w: idx2) std::cout << " " << jugadores[w].getNombre() << "[" << jugadores[w].getInicial() << "]";
            std::cout << "\n";
        }
    }

    std::cout << "\nFin.\n";
    return 0;
}
