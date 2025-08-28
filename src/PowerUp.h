#ifndef POWERUP_H
#define POWERUP_H

#include <string>

enum class PowerUpType {
    DL, TS, BL, PS, LS, ES, UF, AC, NT, EX
};

struct PowerUp {
    PowerUpType tipo;

    explicit PowerUp(PowerUpType t = PowerUpType::DL): tipo(t) {}

    std::string simbolo() const {
        switch (tipo) {
            case PowerUpType::DL: return "DL";
            case PowerUpType::TS: return "TS";
            case PowerUpType::BL: return "BL";
            case PowerUpType::PS: return "PS";
            case PowerUpType::LS: return "LS";
            case PowerUpType::ES: return "ES";
            case PowerUpType::UF: return "UF";
            case PowerUpType::AC: return "AC";
            case PowerUpType::NT: return "NT";
            case PowerUpType::EX: return "EX";
        }
        return "??";
    }

    std::string nombre() const {
        switch (tipo) {
            case PowerUpType::DL: return "Doble Linea";
            case PowerUpType::TS: return "Trampa Secreta";
            case PowerUpType::BL: return "Bloqueo";
            case PowerUpType::PS: return "Pase";
            case PowerUpType::LS: return "Llave Secreta";
            case PowerUpType::ES: return "Escurridizo";
            case PowerUpType::UF: return "Union a Futuro";
            case PowerUpType::AC: return "A Que Costo";
            case PowerUpType::NT: return "Nuevas Tierras";
            case PowerUpType::EX: return "Explosivos";
        }
        return "Desconocido";
    }
};

#endif // POWERUP_H
