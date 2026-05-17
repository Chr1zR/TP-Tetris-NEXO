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
const uint8_t* tetrominoObtenerMatrizRotacion(tTipoTetromino tipo, uint8_t rotacion){

    if(rotacion > 3) rotacion = rotacion % 4;

    switch(tipo){
        case TETROMINO_I:
            return (const uint8_t*)tetromino_I_rotaciones[rotacion];
        case TETROMINO_J:
            return (const uint8_t*)tetromino_J_rotaciones[rotacion];
        case TETROMINO_L:
            return (const uint8_t*)tetromino_L_rotaciones[rotacion];
        case TETROMINO_O:
            return (const uint8_t*)tetromino_O_rotaciones[rotacion];
        case TETROMINO_S:
            return (const uint8_t*)tetromino_S_rotaciones[rotacion];
        case TETROMINO_T:
            return (const uint8_t*)tetromino_T_rotaciones[rotacion];
        case TETROMINO_Z:
            return (const uint8_t*)tetromino_Z_rotaciones[rotacion];
        default:
            return NULL;
    }
}
void tetrominoAleatorio(tTetromino* tetromino, char col){

    if(!tetromino){
        return;
    }

    tetromino->tipo = rand() % 7;
    tetromino->rotacion = 0;

    const uint8_t* matriz = tetrominoObtenerMatrizRotacion(tetromino->tipo, 0);
    if(matriz){
        memcpy(tetromino->matriz, matriz, 16);
    }

    tetromino ->x = rand() % (col -3);
    tetromino ->y = 0;
}
void tetrominoBajar(tTetromino* t){

    t->y++;
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
void tetrominoRotar(tTetromino* tetro, bool sentido_derecha){

    if(!tetro){
        return;
    }

    uint8_t nueva_rotacion;
    if(sentido_derecha){
        nueva_rotacion = (tetro->rotacion + 1) % 4;
    } else {
        nueva_rotacion = (tetro->rotacion + 3) % 4;
    }

    const uint8_t* nueva_matriz = tetrominoObtenerMatrizRotacion(tetro->tipo, nueva_rotacion);
    if(nueva_matriz){
        memcpy(tetro->matriz, nueva_matriz, 16);
        tetro->rotacion = nueva_rotacion;
    }
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
