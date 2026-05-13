#include <GBT/gbt.h>
#include <time.h>
#include "../include/juego.h"
#include "../include/errores.h"
#include "../include/plataforma.h"
#include "../include/logica.h"
#include "../include/config.h"
#include "../include/piezas.h"
#include "../include/paleta.h"

int iniciar_juego(){

    char nombreVentana[128];
    if(!iniciar_plataforma(nombreVentana, sizeof(nombreVentana))){

        return ERROR_INICIALIZACION;
    }
    tGBT_Temporizador* temporizador = gbt_temporizador_crear(0.15);
    if (!temporizador) {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        parar_plataforma(NULL);
        return ERROR_INICIALIZACION;
    }

    tTablero* tablero = crear_tablero(FILAS, COLUMNAS);
    if(!tablero){
        tablero = NULL;
        parar_plataforma(temporizador);
        temporizador = NULL;
        return ERROR_INICIALIZACION;
    }
    srand(time(0));
    tetrominoAleatorio(tablero->pieza_actual, tablero->columnas);
    tetrominoAleatorio(tablero ->pieza_siguiente, tablero->columnas);
    uint8_t corriendo = 1;
    bool game_over = false;
    eGBT_Tecla tecla;

//=============================================
/// DEBUGGING

//    int frame = 0;
//    int actualizaciones = 0;
//=============================================

//    =====================================================
///    BUCLE JUEGO
//    =====================================================
    while(corriendo){

//====================
//        frame++;
//====================

        gbt_procesar_entrada();

        tecla = gbt_obtener_tecla_presionada();

        if (tecla == GBTK_ESCAPE) {

            corriendo = 0;
            printf("Saliendo del ejemplo\n");
        }
        if(tecla == GBTK_DERECHA){
            if(puedeMoverDerecha(tablero->pieza_actual,tablero)){

                tablero->pieza_actual->x++;
            }
        }
        if(tecla == GBTK_IZQUIERDA){

            if(puedeMoverIzquierda(tablero->pieza_actual,tablero)){

                tablero->pieza_actual->x--;
            }
        }

        gbt_borrar_backbuffer(N);

        tableroDibujar(tablero);
        piezasAncladasDibujar(tablero);

        if (gbt_temporizador_consumir(temporizador)) {

//=========================================================
//             actualizaciones++;
//          printf("Frame %d: x=%d, y=%d, tipo=%d\n", frame,
//                 tablero->pieza_actual->x,
//                 tablero->pieza_actual->y,
//                 tablero->pieza_actual->tipo);
//=========================================================
            tableroActualizarEstado(tablero, &game_over);
        }
        if(game_over){
            break;
        }
        piezaActualDibujar(tablero->pieza_actual);

        gbt_volcar_backbuffer();
        gbt_esperar(16);
//============================
//        if(frame > 50) break;
//============================
    }
//====================================================================================
//    printf("Total frames: %d, Actualizaciones: %d\n", frame, actualizaciones);
//====================================================================================
    tTetromino_Destruir(tablero->pieza_actual);
    tTetromino_Destruir(tablero->pieza_siguiente);
    vaciar_tablero(tablero);
    tablero = NULL;
    parar_plataforma(temporizador);
    temporizador = NULL;
    return TODO_OK;
}
