#include "../include/piezas.h"
#include "../include/errores.h"

#include<string.h>
#include<stdlib.h> //rand-srand
tTetromino* tetrominoCrear(){

    tTetromino* tetromino = malloc(sizeof(tTetromino));

    if(!tetromino){
        return NULL;
    }

    tetromino->tipo = TETROMINO_NULO;
    tetromino->x = 0;
    tetromino->y = 0;
    tetromino->rotacion = 0;
    memset(tetromino->matriz, 0, sizeof(tetromino->matriz));

    return tetromino;
}
void tetrominoAleatorio(tTetromino* tetromino, char col){

    if(!tetromino){
        return;
    }

    tetromino->tipo = rand() % 7;

    switch(tetromino->tipo){

        case TETROMINO_I:
            memcpy(tetromino->matriz, tetromino_I, sizeof(tetromino_I));
            break;
        case TETROMINO_J:
            memcpy(tetromino->matriz, tetromino_J, sizeof(tetromino_J));
            break;
        case TETROMINO_L:
            memcpy(tetromino->matriz, tetromino_L, sizeof(tetromino_L));
            break;
        case TETROMINO_O:
            memcpy(tetromino->matriz, tetromino_O, sizeof(tetromino_O));
            break;
        case TETROMINO_S:
            memcpy(tetromino->matriz, tetromino_S, sizeof(tetromino_S));
            break;
        case TETROMINO_T:
            memcpy(tetromino->matriz, tetromino_T, sizeof(tetromino_T));
            break;
        case TETROMINO_Z:
            memcpy(tetromino->matriz, tetromino_Z, sizeof(tetromino_Z));
            break;
        default:

    }
    tetromino ->x = rand() % (col -3);
    tetromino ->y = 0;
}
void tetrominoBajar(tTetromino* t){

    t->y+=TAM_BLOQUE;
}
void tetrominoCopiar(tTetromino* destino, tTetromino* origen){

    if(!destino || !origen){
        return;
    }
    destino->tipo = origen->tipo;
    destino->x = origen->x;
    destino->y = origen->y ;
    destino->rotacion = origen->rotacion;

    memcpy(destino->matriz, origen->matriz, sizeof(origen->matriz));
}
void tTetromino_Destruir(tTetromino* t){

    if(!t){
        return;
    }
    t->x = 0;
    t->y = 0;
    t->rotacion = 0;
    memset(t->matriz, 0, sizeof(t->matriz));
    free(t);
    t = NULL;
}
