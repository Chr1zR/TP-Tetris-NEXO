#include <time.h>
#include <string.h>
#include "../include/juego.h"
#include "../include/errores.h"
#include "../include/plataforma.h"
#include "../include/piezas.h"
#include "../include/paleta.h"
#include "../include/hud.h"
#include "../include/dibujar.h"
#include "../include/velocidad.h"

int iniciar_juego(int argc, char* argv[]){

tJuego* juego = juego_crear(argc, argv);

if(!juego){
    puts("Error al iniciar juego.");
   return ERROR_INICIALIZACION;
}
eGBT_Tecla tecla;

//    ==================================================================
///                             BUCLE JUEGO
//    ==================================================================
    while(juego->corriendo){

//    ==================================================================
///                             PROCESO ENTRADAS
//    ==================================================================
        gbt_procesar_entrada();

        tecla = gbt_obtener_tecla_presionada();

        if(juego->estado_pantalla == ESTADO_JUGANDO){

            if(tecla == GBTK_ESCAPE){
                juego->estado_pantalla = ESTADO_CONFIRMAR_SALIDA;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_p){
                juego->estado_pantalla = ESTADO_PAUSA;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_DERECHA){
                if(puedeMoverDerecha(juego->tablero->pieza_actual,juego->tablero, juego->config)){
                    juego->tablero->pieza_actual->x++;
                    juego->redibujar_hud = 1;
                }
            }
            if(tecla == GBTK_IZQUIERDA){
                if(puedeMoverIzquierda(juego->tablero->pieza_actual,juego->tablero, juego->config)){
                    juego->tablero->pieza_actual->x--;
                    juego->redibujar_hud = 1;
                }
            }
            if(tecla == GBTK_ARRIBA){
                if(puedeRotar(juego->tablero->pieza_actual, juego->tablero, juego->config, true)){
                    tetrominoRotar(juego->tablero->pieza_actual, true);
                    juego->redibujar_hud = 1;
                }
            }
            if(gbt_tecla_sostenida(GBTK_ABAJO) && puedeBajar(juego->tablero->pieza_actual, juego->tablero, juego->config)){
                tetrominoBajar(juego->tablero->pieza_actual);
                juego->estado->puntos++;
                piezaActualDibujar(juego->tablero->pieza_actual, juego->config);
                juego->redibujar_hud = 1;
            }
        }
        else if(juego->estado_pantalla == ESTADO_PAUSA){
            if(tecla == GBTK_p){
                juego->estado_pantalla = ESTADO_JUGANDO;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ARRIBA){
                if(juego->opcion_seleccionada > 0)
                    juego->opcion_seleccionada--;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ABAJO){
                if(juego->opcion_seleccionada < PAUSA_CANT_OPCIONES - 1)
                    juego->opcion_seleccionada++;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ENTER){
                if(juego->opcion_seleccionada == PAUSA_OPCION_CONTINUAR){
                    juego->estado_pantalla = ESTADO_JUGANDO;
                }
                else if(juego->opcion_seleccionada == PAUSA_OPCION_REINICIAR){
                    juego_reiniciar(juego);
                    juego->estado_pantalla = ESTADO_JUGANDO;
                }
                else if(juego->opcion_seleccionada == PAUSA_OPCION_SALIR){
                    juego->corriendo = 0;
                }
                juego->redibujar_hud = 1;
            }
        }
        else if(juego->estado_pantalla == ESTADO_GAME_OVER){
            if(tecla == GBTK_ARRIBA){
                if(juego->opcion_game_over > 0)
                    juego->opcion_game_over--;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ABAJO){
                if(juego->opcion_game_over < GAME_OVER_CANT_OPCIONES - 1)
                    juego->opcion_game_over++;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ENTER){
                if(juego->opcion_game_over == GAME_OVER_OPCION_REINICIAR){
                    juego_reiniciar(juego);
                    juego->estado_pantalla = ESTADO_JUGANDO;
                }
                else if(juego->opcion_game_over == GAME_OVER_OPCION_SALIR){
                    juego->corriendo = 0;
                }
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ESCAPE){
                juego->corriendo = 0;
            }
        }
        else if(juego->estado_pantalla == ESTADO_CONFIRMAR_SALIDA){
            if(tecla == GBTK_s){
                juego->corriendo = 0;
            }
            if(tecla == GBTK_n){
                juego->estado_pantalla = ESTADO_JUGANDO;
                juego->redibujar_hud = 1;
            }
        }

//    ==================================================================
///                             REDIBUJADO PANTALLA
//    ==================================================================
        gbt_borrar_backbuffer(N);

        if(juego->estado_pantalla == ESTADO_JUGANDO){
            if(juego->redibujar_hud){
                fondoHudDibujar(juego->config);
                hudRecuadrosDibujar(juego->config);
                hudTituloNeonDibujar(juego->config);

///                 Dibujar HUD con etiquetas y contenido
                hudNextDibujar(juego->tablero->pieza_siguiente, juego->config);
                hudPiezasDibujar(juego->estado->conteo_piezas, juego->config);
                hudScoreDibujar(juego->estado->puntos, 2, juego->config);
                hudNivelDibujar(juego->estado->nivel, 3, juego->config);
                hudLineasDibujar(juego->estado->lineas_limpias, 2, juego->config);

///                  Dibujar nombre (recuadro NOMBRE al final)
                uint16_t x_nombre = juego->config->hud.disposicion.nombre.offset_x_inicial + 5;
                uint16_t y_nombre = juego->config->hud.disposicion.nombre.offset_y_inicial + 8;
                hudPalabraDibujar(juego->nombre_jugador, x_nombre, y_nombre, CC, 3, juego->config);

                cuadriculaDibujar(juego->tablero, juego->config);
                piezasAncladasDibujar(juego->tablero, juego->config);

                hudTiempoDibujar(juego->seg_transcurrido, 2, juego->config);
            }
            piezaActualDibujar(juego->tablero->pieza_actual, juego->config);

//    ==================================================================
///                             REVISAR ESTADO ACTUAL
//    ==================================================================
            if (gbt_temporizador_consumir(juego->temporizador)) {
                tableroActualizarEstado(juego->tablero, juego->config, juego);
                juego->redibujar_hud = 1;
            }
///             --- TEMPORIZADOR SEGUNDOS TRANSCURRIDOS ---
            if(gbt_temporizador_consumir(juego->temporizador_seg)){
                juego->seg_transcurrido++;
                juego->redibujar_hud = 1;
            }
            if(juego->game_over){
                juego->estado_pantalla = ESTADO_GAME_OVER;
                juego->redibujar_hud = 1;
            }
        }
        else if(juego->estado_pantalla == ESTADO_PAUSA){
            cubrirPausaDibujar(juego->config, juego->opcion_seleccionada);
        }
        else if(juego->estado_pantalla == ESTADO_GAME_OVER){
            cubrirGameOverDibujar(juego->config, juego->estado->puntos, juego->estado->lineas_limpias, juego->nombre_jugador, juego->opcion_game_over);
        }
        else if(juego->estado_pantalla == ESTADO_CONFIRMAR_SALIDA){
            cubrirConfirmarSalidaDibujar(juego->config);
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }
    juego_destruir(juego);
    printf("TODO OK.\n");
    return TODO_OK;

}

tJuego* juego_crear(int argc, char* argv[]) {

    tJuego* juego = calloc(1, sizeof(tJuego));

    if(!juego)
        return NULL;

    //================ CONFIG =================

    if(argc == 1)
        juego->config = crear_config_default();

    else if(argc == 3)
        juego->config = crear_config(argv);

    if(!juego->config){

        juego_destruir(juego);
        return NULL;
    }

    //================ PLATAFORMA =================

    char nombreVentana[128];

    if(!iniciar_plataforma(
            nombreVentana,
            sizeof(nombreVentana),
            juego->config)){

        juego_destruir(juego);
        return NULL;
    }

    //================ TEMPORIZADORES =================

    juego->temporizador =
        gbt_temporizador_crear(INTERVALO_INICIAL);

    if(!juego->temporizador){

        juego_destruir(juego);
        return NULL;
    }

    juego->intervalo_actual = INTERVALO_INICIAL;

    juego->temporizador_seg =
        gbt_temporizador_crear(1);

    if(!juego->temporizador_seg){

        juego_destruir(juego);
        return NULL;
    }

    //================ TABLERO =================

    juego->tablero =
        crear_tablero(juego->config);

    if(!juego->tablero){

        juego_destruir(juego);
        return NULL;
    }

    //================ ESTADO =================

    juego->estado = tEstadoCrear();

    if(!juego->estado){

        juego_destruir(juego);
        return NULL;
    }

    //================ INICIALIZACION =================

    juego->estado->nivel = 1;
    juego->estado->piezas_totales = 0;

    srand(time(0));

    tetrominoAleatorio(
        juego->tablero->pieza_actual,
        juego->config->tablero.columnas
    );

    tetrominoAleatorio(
        juego->tablero->pieza_siguiente,
        juego->config->tablero.columnas
    );

    juego->corriendo = 1;
    juego->game_over = false;
    juego->seg_transcurrido = 0;
    juego->redibujar_hud = 1;
    juego->estado_pantalla = ESTADO_JUGANDO;
    juego->opcion_seleccionada = PAUSA_OPCION_CONTINUAR;
    juego->opcion_game_over = GAME_OVER_OPCION_REINICIAR;

    strcpy(juego->nombre_jugador, "JUGADOR1");

    return juego;
}

void juego_reiniciar(tJuego* juego) {
    if(!juego) return;

    //================ RESET ESTADO =================
    juego->estado->puntos = 0;
    juego->estado->nivel = 1;
    juego->estado->lineas_limpias = 0;
    juego->estado->piezas_totales = 0;
    for(int i = 0; i < 7; i++)
        juego->estado->conteo_piezas[i] = 0;

    //================ RESET TABLERO =================
    tableroReiniciar(juego->tablero, juego->config);

    //================ RESET TEMPORIZADORES =================
    gbt_temporizador_destruir(juego->temporizador);
    juego->temporizador = gbt_temporizador_crear(INTERVALO_INICIAL);
    juego->intervalo_actual = INTERVALO_INICIAL;
    gbt_temporizador_destruir(juego->temporizador_seg);
    juego->temporizador_seg = gbt_temporizador_crear(1);

    //================ RESET PIEZAS =================
    tTetromino_Destruir(juego->tablero->pieza_actual);
    tTetromino_Destruir(juego->tablero->pieza_siguiente);
    juego->tablero->pieza_actual = tetrominoCrear();
    juego->tablero->pieza_siguiente = tetrominoCrear();
    tetrominoAleatorio(
        juego->tablero->pieza_actual,
        juego->config->tablero.columnas
    );
    tetrominoAleatorio(
        juego->tablero->pieza_siguiente,
        juego->config->tablero.columnas
    );

    //================ RESET FLAGS =================
    juego->game_over = false;
    juego->seg_transcurrido = 0;
    juego->redibujar_hud = 1;
    juego->opcion_seleccionada = PAUSA_OPCION_CONTINUAR;
    juego->opcion_game_over = GAME_OVER_OPCION_REINICIAR;
}

void juego_destruir(tJuego* juego) {

    if(!juego)
        return;

    //================ ESTADO =================

    if(juego->estado){
        tEstadoDestruir(juego->estado);
        juego->estado = NULL;
    }

    //================ TABLERO =================

    if(juego->tablero){

        if(juego->tablero->pieza_actual)
            tTetromino_Destruir(juego->tablero->pieza_actual);

        if(juego->tablero->pieza_siguiente)
            tTetromino_Destruir(juego->tablero->pieza_siguiente);

        vaciar_tablero(juego->tablero, juego->config);

        juego->tablero = NULL;
    }

    //================ PLATAFORMA =================

    parar_plataforma(
        juego->temporizador,
        juego->temporizador_seg
    );

    juego->temporizador = NULL;
    juego->temporizador_seg = NULL;

    //================ CONFIG =================

    if(juego->config){
        configDestruir(juego->config);
        juego->config = NULL;
    }

    free(juego);
}
