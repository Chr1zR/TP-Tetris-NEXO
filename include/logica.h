#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include "piezas.h"
#include <stdbool.h>
typedef struct{
    unsigned char **tablero;
    char filas, columnas;
    tTetromino* pieza_actual;
    tTetromino* pieza_siguiente;
    int puntos;
    int lineas_limpias;
}tTablero;

///Funciones tTablero
tTablero* crear_tablero(tConfigPantalla* config);
void vaciar_tablero(tTablero* tablero);

///Funciones dibujar
void fondoDibujar(tTablero* tablero, tConfigPantalla* config);
void bloqueDibujar(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color, tConfigPantalla* config);
void piezasAncladasDibujar(tTablero* t, tConfigPantalla* config);
void piezaActualDibujar(tTetromino* tetro, tConfigPantalla* config);

///Funciones logica
void tableroActualizarEstado(tTablero* tablero, bool* game_over);
bool posicionValida(tTetromino* tetro, tTablero* t);

bool puedeBajar(tTetromino* tetro, tTablero* t);
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t);
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t);
bool puedeRotar(tTetromino* tetro, tTablero* t, bool sentido_derecha);

void piezaAnclar(tTetromino* tetro, tTablero* t);

void lineasLimpiar(tTablero* t);
#endif // LOGICA_H_INCLUDED
