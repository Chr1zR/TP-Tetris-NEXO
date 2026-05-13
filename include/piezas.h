#ifndef PIEZAS_H_INCLUDED
#define PIEZAS_H_INCLUDED
#include <stdint.h>
#include "../include/config.h"  //TAM_BLOQUE
#include "../include/paleta.h"  //define colores

typedef enum {

    TETROMINO_NULO = -1,
    TETROMINO_I,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_O,
    TETROMINO_S,
    TETROMINO_T,
    TETROMINO_Z,
}tTipoTetromino;

typedef struct{
    tTipoTetromino tipo;
    uint8_t matriz[4][4];
    uint8_t rotacion;
    uint16_t x,y;
}tTetromino;

static const uint8_t tetromino_I[4][4] = {
    {0, 0, 0, 0},
    {C, C, C, C},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
static const uint8_t tetromino_J[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {A, 0, 0, 0},
    {A, A, A, 0}
};
static const uint8_t tetromino_L[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, NA},
    {0, NA, NA, NA}
};
static const uint8_t tetromino_O[4][4] = {
    {0, 0, 0, 0},
    {0, AM, AM, 0},
    {0, AM, AM, 0},
    {0, 0, 0, 0}
};
static const uint8_t tetromino_S[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, V, V, 0},
    {V, V, 0, 0}
};
static const uint8_t tetromino_T[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, M, 0, 0},
    {M, M, M, 0}
};
static const uint8_t tetromino_Z[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, R, R, 0},
    {0, 0, R, R}
};


tTetromino* tetrominoCrear();
void tetrominoAleatorio(tTetromino* t, char col);
void tetrominoCopiar(tTetromino* destino, tTetromino* origen);
//void rotar_tetromino(Tetromino* t);
void tTetromino_Destruir(tTetromino* t);
#endif // PIEZAS_H_INCLUDED
