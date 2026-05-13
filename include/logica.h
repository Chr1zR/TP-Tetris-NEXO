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
tTablero* crear_tablero(const char filas, const char columnas);
void vaciar_tablero(tTablero* tablero);

///Funciones dibujar
void tableroDibujar(tTablero* tablero);
void bloqueDibujar(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color);
void piezasAncladasDibujar(tTablero* t);
void piezaActualDibujar(tTetromino* tetro);

///Funciones logica
void tableroActualizarEstado(tTablero* tablero, bool* game_over);
bool posicionValida(tTetromino* tetro, tTablero* t);

bool puedeBajar(tTetromino* tetro, tTablero* t);
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t);
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t);

void piezaAnclar(tTetromino* tetro, tTablero* t);

void lineasLimpiar(tTablero* t);
#endif // LOGICA_H_INCLUDED
