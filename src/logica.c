#include "../include/errores.h"
#include "../include/logica.h"
#include "../include/config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> //printf
//DibujarFondo();
//DibujarTablero();
//DibujarHUD();
///funciones tTablero
tTablero* crear_tablero(const char filas, const char columnas){

    if(filas <= 0 || columnas <= 0){
        return NULL;
    }
    tTablero* tablero = malloc(sizeof(tTablero));

    if(!tablero){
        return NULL;
    }

    tablero->tablero = calloc(filas, sizeof(unsigned char *));
    if(!(tablero -> tablero)){

        free(tablero);
        return NULL;
    }
    for(size_t i = 0; i < filas; i++){

        tablero->tablero[i] = calloc(columnas, sizeof(unsigned char));
        if(!tablero->tablero [i]){
            for(size_t j = 0;j < i ;j++){

                free(tablero->tablero[j]);
            }
            free(tablero->tablero);
            free(tablero);
            return NULL;
        }
    }
    tablero->filas = filas;
    tablero->columnas = columnas;
    tablero->puntos = 0;
    tablero->lineas_limpias = 0;

    tablero->pieza_actual = tetrominoCrear();
    if(!tablero->pieza_actual){

        vaciar_tablero(tablero);
        tablero = NULL;
        return NULL;
    }
    tablero->pieza_siguiente = tetrominoCrear();
    if(!tablero->pieza_siguiente){

        vaciar_tablero(tablero);
        tablero = NULL;
        return NULL;
    }

    return tablero;
}
void vaciar_tablero(tTablero* tablero){

    if(!tablero){
        return;
    }
    if(tablero->tablero){
        for(size_t i = 0; i < tablero->filas; i++){

            free(tablero->tablero[i]);
        }
    free(tablero->tablero);
    tablero->tablero = NULL;
    }
    if(tablero->pieza_actual){

        tTetromino_Destruir(tablero->pieza_actual);
        tablero->pieza_actual = NULL;
    }
    if(tablero->pieza_siguiente){

        tTetromino_Destruir(tablero->pieza_siguiente);
        tablero->pieza_siguiente = NULL;
    }
    free(tablero);
}
void tableroDibujar(tTablero* tablero){

    ///Cuadriacula
    const uint16_t offsetX = 0;
    const uint16_t offsetY = 100;
    uint16_t y;
    uint16_t x;

    for(y = 0; y < ALTO_TABLERO; y++){

        for(x = 0; x < ANCHO_TABLERO; x++){

            gbt_dibujar_pixel(offsetX + x, offsetY + y, N);
        }
    }
    //Lineas verticales
    for(uint16_t col = 0; col < COLUMNAS + 1; col++){
        x = offsetX + (col * TAM_BLOQUE);

        for(y = 0; y < ALTO_TABLERO; y++){

            gbt_dibujar_pixel(x,offsetY + y, CC);
        }

    }
    //Lineas horizontales
    for(uint16_t fila = 0; fila < FILAS + 1; fila++){
        y = offsetY + (fila * TAM_BLOQUE);

        for(x = 0; x < ANCHO_TABLERO; x++){

            gbt_dibujar_pixel(offsetX + x, y, CC);
        }

    }

}
///Funciones dibujar
void bloqueDibujar(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color){

    for(uint8_t i = 0; i < TAM_BLOQUE; i++){

        for(uint8_t j = 0; j < TAM_BLOQUE; j++){

            gbt_dibujar_pixel(x_pantalla + i, y_pantalla + j, color);
        }
    }
}
void piezasAncladasDibujar(tTablero* t){

    if(!t || !(t->tablero)){
        return;
    }

    for(uint8_t i = 0; i < t->filas; i++){

        for(uint8_t j = 0; j < t->columnas; j++){

            if(t->tablero[i][j] != 0){

                bloqueDibujar(j * TAM_BLOQUE, ALTO_VGA_VENTANA - ALTO_TABLERO + i * TAM_BLOQUE, t->tablero[i][j]);
            }
        }
    }
}
void piezaActualDibujar(tTetromino* tetro){

    if(!tetro){
        return;
    }
    for(uint8_t i = 0; i < 4; i++){

        for(uint8_t j = 0; j < 4; j++){

            if(tetro->matriz[i][j] != 0){

                bloqueDibujar((tetro->x + j) * TAM_BLOQUE, ALTO_VGA_VENTANA - ALTO_TABLERO + (tetro->y + i)* TAM_BLOQUE, tetro->matriz[i][j]);
            }
        }
    }
}
///Funciones logica
void tableroActualizarEstado(tTablero* t, bool* game_over){

    if(!t || !t->pieza_actual){
        return;
    }

//=======================================================
    printf("=== Antes: pieza actual: tipo=%d, x=%d, y=%d\n",
             t->pieza_actual->tipo, t->pieza_actual->x, t->pieza_actual->y);
//=======================================================
    t->pieza_actual->y++;

    if(posicionValida(t->pieza_actual, t)){

//=======================================================
        printf("    Posicion valida, continua bajando\n");
//=======================================================
        return;
    }

//=======================================================
    printf("    Posicion INVALIDA, ancla\n");
//=======================================================

    t->pieza_actual->y--;

    piezaAnclar(t->pieza_actual, t);

//=======================================================
    printf("    Pieza anclada en y=%d\n", t->pieza_actual->y);
//=======================================================

    lineasLimpiar(t);

    tetrominoCopiar(t->pieza_actual, t->pieza_siguiente);

//=======================================================
     printf("    Nueva pieza actual (era siguiente): tipo=%d, x=%d, y=%d\n",
             t->pieza_actual->tipo, t->pieza_actual->x, t->pieza_actual->y);
//=======================================================

    tetrominoAleatorio(t->pieza_siguiente, t->columnas);

//=======================================================
    printf("    Nueva pieza siguiente generada: tipo=%d, x=%d\n",
             t->pieza_siguiente->tipo, t->pieza_siguiente->x);
//=======================================================



    if(!posicionValida(t->pieza_actual, t)){

//=======================================================
    printf("    ERROR: Nueva pieza actual NO es valida! GAME OVER\n");
//=======================================================

        *game_over = true;
        return;
    }
//=======================================================
    printf("    Nueva pieza actual es valida\n\n");
//=======================================================
}
bool posicionValida(tTetromino* tetro, tTablero* t){

    if(!tetro || !t){
        return false;
    }

    int x_tablero, y_tablero;

    for(uint8_t i = 0; i < 4; i++){

        for(uint8_t j = 0; j < 4; j++){

            if(tetro->matriz[i][j]){

                x_tablero = tetro->x + j;
                y_tablero = tetro->y + i;

                // Validacion limites
                if(x_tablero < 0 || x_tablero >= t->columnas ||
                    y_tablero < 0 || y_tablero >= t->filas){
                    return false;
                }

                // colisión
                if(t->tablero[y_tablero][x_tablero] != 0){
                    return false;
                }
            }
        }
    }

    return true;
}
///Funciones validantes
bool puedeBajar(tTetromino* tetro, tTablero* t){

    if(!tetro || !t){
        return false;
    }
    tetro->y++;

    bool valido = posicionValida(tetro, t);

    tetro->y--;

    return valido;
}
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t){

    if(!tetro || !t){
        return false;
    }

    tetro->x--;

    bool valido = posicionValida(tetro, t);

    tetro->x++;

    return valido;
}
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t){

    if(!tetro || !t){
        return false;
    }
    tetro->x++;

    bool valido = posicionValida(tetro, t);

    tetro->x--;

    return valido;
}

void piezaAnclar(tTetromino* tetro, tTablero* t){

    uint8_t i,j,x,y;


    for(i = 0; i < 4; i++){

        for(j = 0; j < 4; j++){

            if(tetro->matriz[i][j]){

                x = tetro->x + j;
                y = tetro->y + i;

                if(x >= t->columnas || y >= t->filas){

                    return; //Si el bloque no cabe, cancelamos anclaje
                }
            }
        }
    }

    for(i = 0; i < 4; i++){

        for(j = 0; j < 4; j++){

            if(tetro->matriz[i][j]){

                x = tetro->x + j;
                y = tetro->y + i;
                t->tablero[y][x] = tetro->matriz[i][j];
            }
        }
    }
}
void lineasLimpiar(tTablero* t){

    if(!t){
        return;
    }

    bool llena;

    for(int y = 0; y < t->filas; y++){

        llena = true;

        // verificar fila completa
        for(int x = 0; x < t->columnas; x++){

            if(t->tablero[y][x] == 0){

                llena = false;
                break;
            }
        }

        if(llena){

            // guardar puntero de la fila eliminada
            unsigned char* filaEliminada = t->tablero[y];

            // mover punteros hacia abajo
            for(int fila = y; fila > 0; fila--){

                t->tablero[fila] = t->tablero[fila - 1];
            }

            // reutilizar la fila eliminada arriba
            t->tablero[0] = filaEliminada;

            // vaciar contenido
            memset(t->tablero[0], 0, t->columnas * sizeof(unsigned char));

            t->lineas_limpias++;

            y--;
        }
    }
}
