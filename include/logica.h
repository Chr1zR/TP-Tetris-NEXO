#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include "piezas.h"
#include <stdbool.h>

typedef struct sJuego tJuego;
typedef struct{
    unsigned char **tablero;
    tTetromino* pieza_actual;
    tTetromino* pieza_siguiente;
}tTablero;

typedef struct{

    uint32_t puntos;
    uint16_t lineas_limpias;
    uint16_t conteo_piezas[7];
    uint16_t piezas_totales;
    uint8_t nivel;
}tEstadoJuego;

///Funciones tTablero
tTablero* crear_tablero(tConfigPantalla* config);
void vaciar_tablero(tTablero* tablero, tConfigPantalla* config);
void tableroReiniciar(tTablero* tablero, tConfigPantalla* config);

///Funciones tEstadoJuego
tEstadoJuego* tEstadoCrear();
void tEstadoDestruir(tEstadoJuego* estado);


///Funciones dibujar
void fondoHudDibujar(tConfigPantalla* config);
void cuadriculaDibujar(tTablero* tablero, tConfigPantalla* config);
void piezasAncladasDibujar(tTablero* t, tConfigPantalla* config);
void piezaActualDibujar(tTetromino* tetro, tConfigPantalla* config);

///Funciones logica
void tableroActualizarEstado(tTablero* tablero, tConfigPantalla* config, tJuego* juego);
bool posicionValida(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

bool puedeBajar(tTetromino* tetro, tTablero* t, tConfigPantalla* config);
bool piezaTocoFondo(tTetromino* tetro, tTablero* t, tConfigPantalla* config);
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t, tConfigPantalla* config);
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t, tConfigPantalla* config);
bool puedeRotar(tTetromino* tetro, tTablero* t, tConfigPantalla* config, bool sentido_derecha);

void piezaAnclar(tTetromino* tetro, tTablero* t, tConfigPantalla* config);
void piezaAnclarYContinuar(tTablero* t, tConfigPantalla* config, tJuego* juego);

int lineasLimpiar(tTablero* t, tConfigPantalla* config, tEstadoJuego* estado);
#endif // LOGICA_H_INCLUDED
