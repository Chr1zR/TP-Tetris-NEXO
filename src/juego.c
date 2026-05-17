#include <GBT/gbt.h>
#include <time.h>
#include "../include/juego.h"
#include "../include/errores.h"
#include "../include/plataforma.h"
#include "../include/logica.h"
#include "../include/config.h"
#include "../include/piezas.h"
#include "../include/paleta.h"
#include "../include/hud.h"

int iniciar_juego(int argc, char* argv[]){

//====================================================================
    ///Configuracion inicial modalidad juego
    tConfigPantalla* config;

    if(argc == 1){
        //Si no detecta argumentos pasados ejecutamos config_default -> VGA con escala=1
        config = crear_config_default();
    }
    if(argc == 3){

        config = crear_config(argv);
    }
    if(!config){

        puts("Error al crear configuracion del juego. Saliendo...\n");
        return ERROR_INICIALIZACION;
    }
//=====================================================================
    ///Configuracion Ventana grafica
    char nombreVentana[128];
    if(!iniciar_plataforma(nombreVentana, sizeof(nombreVentana), config)){

        configDestruir(config);
        return ERROR_INICIALIZACION;
    }
//=====================================================================
    ///Configuracion variables gestion bucle de juego
    tGBT_Temporizador* temporizador = gbt_temporizador_crear(0.2);
    if (!temporizador) {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        parar_plataforma(NULL,NULL);
        configDestruir(config);
        config = NULL;
        return ERROR_INICIALIZACION;
    }

    tGBT_Temporizador* temporizador_seg = gbt_temporizador_crear(1);
    if (!temporizador_seg) {
        fprintf(stderr, "Error al crear el temporizador para el tiempo: %s\n", gbt_obtener_log());
        parar_plataforma(temporizador, NULL);
        configDestruir(config);
        config = NULL;
        return ERROR_INICIALIZACION;
    }

    tTablero* tablero = crear_tablero(config);
    if(!tablero){
        tablero = NULL;
        parar_plataforma(temporizador,temporizador_seg);
        configDestruir(config);
        config = NULL;
        return ERROR_INICIALIZACION;
    }
    srand(time(0));
    tetrominoAleatorio(tablero->pieza_actual, tablero->columnas);
    tetrominoAleatorio(tablero ->pieza_siguiente, tablero->columnas);
    uint8_t corriendo = 1;
    bool game_over = false;
    eGBT_Tecla tecla;

    static uint16_t seg_transcurrido = 0;

///          ---Contadores de juego---
      uint32_t score = 0;
      uint8_t nivel = 1;
      uint16_t lineas_totales = 0;
      char nombre_jugador[20] = "JUGADOR1";  // Placeholder

//=============================================
/// DEBUGGING

//    int frame = 0;
//    int actualizaciones = 0;
//=============================================

//    ==================================================================
///                             BUCLE JUEGO
//    ==================================================================
    while(corriendo){

//====================
//        frame++;
        printf("DEBUG: Dentro del bucle, tecla anterior = %d\n", tecla);
//====================

//    ==================================================================
///                             PROCESO ENTRADAS
//    ==================================================================
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
        if(tecla == GBTK_ARRIBA){

//================================================================
        printf("DEBUG: Tecla ARRIBA presionada\n");
        printf("DEBUG: puedeRotar = %d\n", puedeRotar(tablero->pieza_actual, tablero, true));
        fflush(stdout);
//================================================================
            if(puedeRotar(tablero->pieza_actual, tablero, true)){

//================================================================
                printf("DEBUG: Llamando tetrominoRotar\n");
//================================================================

                tetrominoRotar(tablero->pieza_actual, true);
//================================================================
                printf("DEBUG: Nueva rotacion = %d\n", tablero->pieza_actual->rotacion);
//================================================================
            }
        }

        if(gbt_tecla_sostenida(GBTK_ABAJO) && puedeBajar(tablero->pieza_actual, tablero)){

            tetrominoBajar(tablero->pieza_actual);
            piezaActualDibujar(tablero->pieza_actual, config);
        }
//    ==================================================================
///                             REDIBUJADO PANTALLA
//    ==================================================================
        gbt_borrar_backbuffer(N);

        hudRecuadrosDibujar(config);

///                 Dibujar HUD con etiquetas y contenido
          hudNextDibujar(config);
          hudScoreConEtiquetaDibujar(score, 2, config);
          hudNivelConEtiquetaDibujar(nivel, 2, config);
          hudLineasConEtiquetaDibujar(lineas_totales, 2, config);

///                  Dibujar nombre (recuadro NOMBRE al final)
          uint16_t x_nombre = config->hud.nombre.offset_x_inicial + 5;
          uint16_t y_nombre = config->hud.nombre.offset_y_inicial + 8;
          hudPalabraDibujar(nombre_jugador, x_nombre, y_nombre, CC, 3, config);

          fondoDibujar(tablero, config);


        fondoDibujar(tablero, config);
        piezasAncladasDibujar(tablero, config);

//    ==================================================================
///                             REVISAR ESTADO ACTUAL
//    ==================================================================
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
///             --- TEMPORIZADOR SEGUNDOS TRANSCURRIDOS ---
        if(gbt_temporizador_consumir(temporizador_seg)){
            seg_transcurrido++;
        }
        if(game_over){
            printf("GAME OVER!. Saliendo...\n");
            corriendo = 0;
        }
        hudTiempoDibujar(seg_transcurrido, 2, config);
        piezaActualDibujar(tablero->pieza_actual, config);

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
    parar_plataforma(temporizador, temporizador_seg);
    configDestruir(config);
    config = NULL;
    printf("TODO OK.\n");
    return TODO_OK;

}
