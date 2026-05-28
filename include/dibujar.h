#ifndef DIBUJAR_H_INCLUDED
#define DIBUJAR_H_INCLUDED

#include <stdint.h>
#include <GBT/gbt.h>
#include "config.h"

typedef struct sTetromino tTetromino;
typedef struct sTablero tTablero;

/**
 * @desc Dibuja un pixel si esta dentro de los limites.
 * @param x uint16_t Coordenada X.
 * @param y uint16_t Coordenada Y.
 * @param color uint8_t Indice de color.
 * @param ancho uint16_t Ancho limite de la ventana.
 * @param alto uint16_t Alto limite de la ventana.
 * @return void
 */
static inline void pixelDibujar(uint16_t x, uint16_t y, uint8_t color, uint16_t ancho, uint16_t alto) {
    if (x < ancho && y < alto)
        gbt_dibujar_pixel(x, y, color);
}

/**
 * @desc Dibuja una linea horizontal.
 * @param x uint16_t Coordenada X inicial.
 * @param y uint16_t Coordenada Y.
 * @param longitud uint16_t Longitud en pixeles.
 * @param color uint8_t Indice de color.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void lineaHDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color, tConfigPantalla* config);

/**
 * @desc Dibuja una linea vertical.
 * @param x uint16_t Coordenada X.
 * @param y uint16_t Coordenada Y inicial.
 * @param longitud uint16_t Longitud en pixeles.
 * @param color uint8_t Indice de color.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void lineaVDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color, tConfigPantalla* config);

/**
 * @desc Dibuja el borde de un rectangulo.
 * @param x uint16_t Coordenada X inicial.
 * @param y uint16_t Coordenada Y inicial.
 * @param ancho uint16_t Ancho del rectangulo.
 * @param alto uint16_t Alto del rectangulo.
 * @param color uint8_t Indice de color.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void rectanguloDibujar(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color, tConfigPantalla* config);

/**
 * @desc Dibuja un rectangulo relleno.
 * @param x uint16_t Coordenada X inicial.
 * @param y uint16_t Coordenada Y inicial.
 * @param ancho uint16_t Ancho del rectangulo.
 * @param alto uint16_t Alto del rectangulo.
 * @param color uint8_t Indice de color.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void rectanguloRelleno(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color, tConfigPantalla* config);

/**
 * @desc Dibuja un bitmap escalado en pantalla.
 * @param bitmap const uint8_t* Puntero a datos del bitmap.
 * @param x uint16_t Coordenada X inicial.
 * @param y uint16_t Coordenada Y inicial.
 * @param ancho uint8_t Ancho del bitmap.
 * @param alto uint8_t Alto del bitmap.
 * @param color uint8_t Indice de color.
 * @param escala uint8_t Factor de escala.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void bitmapDibujar(const uint8_t* bitmap, uint16_t x, uint16_t y, uint8_t ancho, uint8_t alto,
                    uint8_t color, uint8_t escala, tConfigPantalla* config);

/**
 * @desc Dibuja la cuadricula del tablero.
 * @param tablero tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void cuadriculaDibujar(tTablero* tablero, tConfigPantalla* config);

/**
 * @desc Dibuja las piezas ya ancladas en el tablero.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void piezasAncladasDibujar(tTablero* t, tConfigPantalla* config);

/**
 * @desc Dibuja la pieza actual en movimiento.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void piezaActualDibujar(tTetromino* tetro, tConfigPantalla* config);

/**
 * @desc Dibuja la pieza fantasma en su posicion final.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param ghost_y int Posicion Y final calculada.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void piezaGhostDibujar(tTetromino* tetro, int ghost_y, tConfigPantalla* config);

#endif // DIBUJAR_H_INCLUDED
