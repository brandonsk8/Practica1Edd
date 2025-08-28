# Practica1Edd

Juego de "puntos y cuadros" en consola con soporte de power-ups.
Codigo en C++17, compilacion por Makefile.

## Requisitos

- Compilador C++17 (g++)
- Make
- Probado en:
  - Windows 10/11 con MSYS2 MinGW 64-bit
  - Linux (Debian/Ubuntu) y macOS (Xcode CLT)

## Estructura del proyecto

```
Practica1Edd/
Makefile
bin/                  (se crea al compilar)
 src/
   main.cpp
   PowerUp.{h,cpp}
   PilaPowerUps.{h,cpp}
   Jugador.{h,cpp}
   ColaTurnos.{h,cpp}
    ListaEnlazada.h
    Tablero.{h,cpp}
    ...
```

## Windows (MSYS2 MinGW 64-bit)

1) Instalar MSYS2 (https://www.msys2.org)
2) Abrir la terminal **MSYS2 MinGW 64-bit** y ejecutar:
   ```
   pacman -Syu
   pacman -S --needed base-devel mingw-w64-x86_64-toolchain
   ```
   (esto instala g++ y mingw32-make)

3) Abrir PowerShell o la misma terminal y ubicarse en la carpeta del proyecto:
   ```
   cd C:\Users\bRANDON\Desktop\Practica1Edd
   ```

4) Compilar:
   ```
   mingw32-make
   ```

5) Ejecutar:
   ```
   .\bin\totito.exe
   ```

6) Otras tareas utiles:
   ```
   mingw32-make clean       # limpia .o y .d
   mingw32-make -B          # fuerza recompilacion completa
   mingw32-make debug       # build con simbolos (-g) y O0
   mingw32-make deepclean   # limpia y borra el ejecutable
   ```


## Linux / macOS

1) Instalar herramientas
   - Debian/Ubuntu:
     ```
     sudo apt update
     sudo apt install build-essential
     ```
   - macOS (Xcode Command Line Tools):
     ```
     xcode-select --install
     ```

2) Compilar y ejecutar:
   ```
   make
   ./bin/totito
   ```

3) Limpieza y rebuild:
   ```
   make clean
   make -B
   ```

## Ejecucion rapida

Al iniciar, el programa pide:
- Tamano del tablero (min 3)
- Numero de jugadores
- Nombre e inicial de cada jugador (o * para asignar A/B/C)

Comandos en el juego:
- `r1 c1 r2 c2`  trazar linea entre puntos adyacentes 
- `u`            usar power-up del tope
- `p`            atajo de Pase si PS esta en el tope
- `v`            alternar modo clarividente
- `h`            ayuda
- `-1`           salir



