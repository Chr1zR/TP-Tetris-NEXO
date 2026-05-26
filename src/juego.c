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
#include "../include/ranking.h"

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

        if(juego->estado_pantalla == ESTADO_PRESENTACION){
            if(tecla == GBTK_ESCAPE){
                juego->corriendo = 0;
            }
            if(tecla == GBTK_ARRIBA){
                if(juego->opcion_presentacion > 0)
                    juego->opcion_presentacion--;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ABAJO){
                if(juego->opcion_presentacion < PRESENTACION_CANT_OPCIONES - 1)
                    juego->opcion_presentacion++;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ENTER){
                if(juego->opcion_presentacion == PRESENTACION_OPCION_START){
                    juego->estado_pantalla = ESTADO_INGRESAR_NOMBRE;
                    juego->cursor_nombre = 0;
                    juego->buffer_nombre[0] = '\0';
                    juego->frame_cursor = 0;
                    juego->cursor_visible = true;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_presentacion == PRESENTACION_OPCION_CARGAR){
                    juego->estado_pantalla = ESTADO_CARGAR_PARTIDA;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_presentacion == PRESENTACION_OPCION_PUNTAJES){
                    juego->estado_pantalla = ESTADO_PUNTAJES;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_presentacion == PRESENTACION_OPCION_OPCIONES){
                    juego->estado_pantalla = ESTADO_MENU_OPCIONES;
                    juego->opcion_opciones = OPCIONES_OPCION_MODALIDAD;
                    juego->opcion_seleccion_modalidad = juego->config->modo_juego;
                    juego->opcion_seleccion_columnas_dx = juego->config->tablero.columnas;
    juego->opcion_seleccion_velocidad = juego->config->velocidad_inicial;
    juego->confirmacion_guardado = 0;
    juego->confirmar_borrar_ranking = 0;
}
                else if(juego->opcion_presentacion == PRESENTACION_OPCION_ACERCA_DE){
                    juego->estado_pantalla = ESTADO_ACERCA_DE;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_presentacion == PRESENTACION_OPCION_SALIR){
                    juego->corriendo = 0;
                }
                else{
                    juego->redibujar_hud = 1;
                }
            }
        }
        else if(juego->estado_pantalla == ESTADO_JUGANDO){

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
                    if(juego->en_tolerancia_fijacion){
                        gbt_temporizador_destruir(juego->temporizador_fijacion);
                        juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
                    }
                    juego->redibujar_hud = 1;
                }
            }
            if(tecla == GBTK_IZQUIERDA){
                if(puedeMoverIzquierda(juego->tablero->pieza_actual,juego->tablero, juego->config)){
                    juego->tablero->pieza_actual->x--;
                    if(juego->en_tolerancia_fijacion){
                        gbt_temporizador_destruir(juego->temporizador_fijacion);
                        juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
                    }
                    juego->redibujar_hud = 1;
                }
            }
            if(tecla == GBTK_ARRIBA){
                if(puedeRotar(juego->tablero->pieza_actual, juego->tablero, juego->config, true)){
                    tetrominoRotar(juego->tablero->pieza_actual, true);
                    if(juego->en_tolerancia_fijacion){
                        gbt_temporizador_destruir(juego->temporizador_fijacion);
                        juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
                    }
                    juego->redibujar_hud = 1;
                }
            }
            if(gbt_tecla_sostenida(GBTK_ABAJO) && puedeBajar(juego->tablero->pieza_actual, juego->tablero, juego->config)){
                tetrominoBajar(juego->tablero->pieza_actual);
                juego->estado->puntos++;
                if(juego->en_tolerancia_fijacion){
                    gbt_temporizador_destruir(juego->temporizador_fijacion);
                    juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
                }
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
                    if(juego->ranking && juego->estado->puntos > 0){
                        ranking_agregar(juego->ranking, juego->nombre_jugador,
                                       juego->estado->puntos, juego->estado->lineas_limpias);
                    }
                    juego_reiniciar(juego);
                    juego->estado_pantalla = ESTADO_JUGANDO;
                }
                else if(juego->opcion_game_over == GAME_OVER_OPCION_SALIR){
                    if(juego->ranking && juego->estado->puntos > 0){
                        ranking_agregar(juego->ranking, juego->nombre_jugador,
                                       juego->estado->puntos, juego->estado->lineas_limpias);
                    }
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
                if(juego->ranking && juego->estado->puntos > 0){
                    ranking_agregar(juego->ranking, juego->nombre_jugador,
                                   juego->estado->puntos, juego->estado->lineas_limpias);
                }
                juego->corriendo = 0;
            }
            if(tecla == GBTK_n){
                juego->estado_pantalla = ESTADO_JUGANDO;
                juego->redibujar_hud = 1;
            }
        }
        else if(juego->estado_pantalla == ESTADO_MENU_OPCIONES){
            if(juego->confirmacion_guardado > 0 && juego->confirmacion_guardado < 255)
                juego->confirmacion_guardado--;

            if(tecla == GBTK_ESCAPE){
                juego->estado_pantalla = ESTADO_PRESENTACION;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ARRIBA){
                if(juego->opcion_opciones > 0)
                    juego->opcion_opciones--;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_ABAJO){
                if(juego->opcion_opciones < OPCIONES_CANT_OPCIONES - 1)
                    juego->opcion_opciones++;
                juego->redibujar_hud = 1;
            }
            if(tecla == GBTK_DERECHA){
                if(juego->opcion_opciones == OPCIONES_OPCION_MODALIDAD){
                    juego->opcion_seleccion_modalidad = 1 - juego->opcion_seleccion_modalidad;
                    if(juego->opcion_seleccion_modalidad == 0)
                        juego->opcion_seleccion_columnas_dx = 10;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_opciones == OPCIONES_OPCION_COLUMNAS){
                    if(juego->opcion_seleccion_modalidad == 1){
                        if(juego->opcion_seleccion_columnas_dx < 16)
                            juego->opcion_seleccion_columnas_dx++;
                        juego->redibujar_hud = 1;
                    }
                }
                else if(juego->opcion_opciones == OPCIONES_OPCION_VELOCIDAD){
                    juego->opcion_seleccion_velocidad += 0.1;
                    if(juego->opcion_seleccion_velocidad > 2.0)
                        juego->opcion_seleccion_velocidad = 2.0;
                    juego->redibujar_hud = 1;
                }
            }
            if(tecla == GBTK_IZQUIERDA){
                if(juego->opcion_opciones == OPCIONES_OPCION_MODALIDAD){
                    juego->opcion_seleccion_modalidad = 1 - juego->opcion_seleccion_modalidad;
                    if(juego->opcion_seleccion_modalidad == 0)
                        juego->opcion_seleccion_columnas_dx = 10;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_opciones == OPCIONES_OPCION_COLUMNAS){
                    if(juego->opcion_seleccion_modalidad == 1){
                        if(juego->opcion_seleccion_columnas_dx > 8)
                            juego->opcion_seleccion_columnas_dx--;
                        juego->redibujar_hud = 1;
                    }
                }
                else if(juego->opcion_opciones == OPCIONES_OPCION_VELOCIDAD){
                    juego->opcion_seleccion_velocidad -= 0.1;
                    if(juego->opcion_seleccion_velocidad < 0.2)
                        juego->opcion_seleccion_velocidad = 0.2;
                    juego->redibujar_hud = 1;
                }
            }
            if(tecla == GBTK_ENTER){
                if(juego->opcion_opciones == OPCIONES_OPCION_GUARDAR){
                    juego->config->modo_juego = (tModoJuego)juego->opcion_seleccion_modalidad;
                    juego->config->tablero.columnas = juego->opcion_seleccion_columnas_dx;
                    anchosCalcular(juego->config);
                    juego->config->velocidad_inicial = juego->opcion_seleccion_velocidad;
                    if(configGuardar(juego->config, "tetris.cfg") == 0)
                        juego->confirmacion_guardado = 90;
                    else
                        juego->confirmacion_guardado = 255;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_opciones == OPCIONES_OPCION_VOLVER){
                    juego->estado_pantalla = ESTADO_PRESENTACION;
                    juego->redibujar_hud = 1;
                }
                else if(juego->opcion_opciones == OPCIONES_OPCION_VELOCIDAD){
                    juego->opcion_seleccion_velocidad += 0.1;
                    if(juego->opcion_seleccion_velocidad > 2.0)
                        juego->opcion_seleccion_velocidad = 2.0;
                    juego->redibujar_hud = 1;
                }
            }
        }
        else if(juego->estado_pantalla == ESTADO_ACERCA_DE){
            if(tecla == GBTK_ESCAPE){
                juego->estado_pantalla = ESTADO_PRESENTACION;
                juego->redibujar_hud = 1;
            }
        }
        else if(juego->estado_pantalla == ESTADO_CARGAR_PARTIDA){
            if(tecla == GBTK_ESCAPE){
                juego->estado_pantalla = ESTADO_PRESENTACION;
                juego->redibujar_hud = 1;
            }
        }
        else if(juego->estado_pantalla == ESTADO_PUNTAJES){
            if(juego->confirmar_borrar_ranking){
                if(tecla == GBTK_s){
                    if(juego->ranking)
                        ranking_borrar(juego->ranking);
                    juego->confirmar_borrar_ranking = 0;
                    juego->redibujar_hud = 1;
                }
                if(tecla == GBTK_n || tecla == GBTK_ESCAPE){
                    juego->confirmar_borrar_ranking = 0;
                    juego->redibujar_hud = 1;
                }
            } else {
                if(tecla == GBTK_ESCAPE){
                    juego->estado_pantalla = ESTADO_PRESENTACION;
                    juego->redibujar_hud = 1;
                }
                if(tecla == GBTK_r){
                    if(juego->ranking && ranking_tamanio(juego->ranking) > 0){
                        juego->confirmar_borrar_ranking = 1;
                        juego->redibujar_hud = 1;
                    }
                }
            }
        }
        else if(juego->estado_pantalla == ESTADO_INGRESAR_NOMBRE){
            juego->frame_cursor++;
            if(juego->frame_cursor % 30 == 0)
                juego->cursor_visible = !juego->cursor_visible;

            if(tecla == GBTK_ENTER){
                if(juego->cursor_nombre > 0){
                    juego->buffer_nombre[juego->cursor_nombre] = '\0';
                    strcpy(juego->nombre_jugador, juego->buffer_nombre);
                    juego->estado_pantalla = ESTADO_JUGANDO;
                    juego->redibujar_hud = 1;
                }
            }
            else if(tecla == GBTK_RETROCESO){
                if(juego->cursor_nombre > 0){
                    juego->cursor_nombre--;
                    juego->buffer_nombre[juego->cursor_nombre] = '\0';
                    juego->redibujar_hud = 1;
                }
            }
            else if(tecla >= GBTK_a && tecla <= GBTK_z){
                if(juego->cursor_nombre < 19){
                    char c = (char)('A' + (tecla - GBTK_a));
                    juego->buffer_nombre[juego->cursor_nombre] = c;
                    juego->cursor_nombre++;
                    juego->buffer_nombre[juego->cursor_nombre] = '\0';
                    juego->redibujar_hud = 1;
                }
            }
            else if(tecla >= GBTK_0 && tecla <= GBTK_9){
                if(juego->cursor_nombre < 19){
                    char c = (char)('0' + (tecla - GBTK_0));
                    juego->buffer_nombre[juego->cursor_nombre] = c;
                    juego->cursor_nombre++;
                    juego->buffer_nombre[juego->cursor_nombre] = '\0';
                    juego->redibujar_hud = 1;
                }
            }
            else if(tecla == GBTK_ESPACIO){
                if(juego->cursor_nombre < 19){
                    juego->buffer_nombre[juego->cursor_nombre] = ' ';
                    juego->cursor_nombre++;
                    juego->buffer_nombre[juego->cursor_nombre] = '\0';
                    juego->redibujar_hud = 1;
                }
            }
            else if(tecla == GBTK_ESCAPE){
                strcpy(juego->buffer_nombre, "JUGADOR1");
                juego->cursor_nombre = 8;
                juego->buffer_nombre[juego->cursor_nombre] = '\0';
                strcpy(juego->nombre_jugador, juego->buffer_nombre);
                juego->estado_pantalla = ESTADO_JUGANDO;
                juego->redibujar_hud = 1;
            }
        }

//    ==================================================================
///                             REDIBUJADO PANTALLA
//    ==================================================================
        gbt_borrar_backbuffer(N);

        if(juego->estado_pantalla == ESTADO_PRESENTACION){
            presentarPantallaDibujar(juego->config, juego->opcion_presentacion);
        }
        else if(juego->estado_pantalla == ESTADO_JUGANDO){
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
                if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config)){
                    juego->en_tolerancia_fijacion = true;
                    juego->intervalo_fijacion = juego->intervalo_actual * 0.5;
                    if(juego->temporizador_fijacion){
                        gbt_temporizador_destruir(juego->temporizador_fijacion);
                    }
                    juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
                }
                juego->redibujar_hud = 1;
            }
            if(juego->en_tolerancia_fijacion && gbt_temporizador_consumir(juego->temporizador_fijacion)){
                piezaAnclarYContinuar(juego->tablero, juego->config, juego);
                juego->en_tolerancia_fijacion = false;
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
        else if(juego->estado_pantalla == ESTADO_MENU_OPCIONES){
            menuConfiguracionDibujar(juego->config, juego->opcion_opciones,
                juego->opcion_seleccion_modalidad,
                juego->opcion_seleccion_columnas_dx,
                juego->opcion_seleccion_velocidad,
                juego->confirmacion_guardado);
        }
        else if(juego->estado_pantalla == ESTADO_ACERCA_DE){
            pantallaAcercaDeDibujar(juego->config);
        }
        else if(juego->estado_pantalla == ESTADO_CARGAR_PARTIDA){
            pantallaCargarPartidaDibujar(juego->config);
        }
        else if(juego->estado_pantalla == ESTADO_PUNTAJES){
            pantallaPuntajesDibujar(juego->config, juego->ranking, juego->confirmar_borrar_ranking);
        }
        else if(juego->estado_pantalla == ESTADO_INGRESAR_NOMBRE){
            pantallaIngresarNombreDibujar(juego->config, juego->buffer_nombre,
                juego->cursor_nombre, juego->cursor_visible);
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

    juego->config = calloc(1, sizeof(tConfigPantalla));
    if(!juego->config){
        juego_destruir(juego);
        return NULL;
    }

    if(configCargar(juego->config, "tetris.cfg") != 0){
        free(juego->config);
        juego->config = NULL;
        if(argc == 1)
            juego->config = crear_config_default();
        else if(argc == 3)
            juego->config = crear_config(argv);
        else {
            juego_destruir(juego);
            return NULL;
        }
    }

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
        gbt_temporizador_crear(juego->config->velocidad_inicial);

    if(!juego->temporizador){

        juego_destruir(juego);
        return NULL;
    }

    juego->intervalo_actual = juego->config->velocidad_inicial;

    juego->temporizador_fijacion = NULL;
    juego->en_tolerancia_fijacion = false;
    juego->intervalo_fijacion = juego->config->velocidad_inicial * 0.5;

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
    juego->estado_pantalla = ESTADO_PRESENTACION;
    juego->opcion_seleccionada = PAUSA_OPCION_CONTINUAR;
    juego->opcion_game_over = GAME_OVER_OPCION_REINICIAR;
    juego->opcion_presentacion = PRESENTACION_OPCION_START;

strcpy(juego->nombre_jugador, "JUGADOR1");

    juego->ranking = ranking_crear(10);
    if (juego->ranking)
        ranking_cargar(juego->ranking);

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
    juego->temporizador = gbt_temporizador_crear(juego->config->velocidad_inicial);
    juego->intervalo_actual = juego->config->velocidad_inicial;
    if(juego->temporizador_fijacion){
        gbt_temporizador_destruir(juego->temporizador_fijacion);
    }
    juego->temporizador_fijacion = NULL;
    juego->en_tolerancia_fijacion = false;
    juego->intervalo_fijacion = juego->config->velocidad_inicial * 0.5;
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
    juego->opcion_presentacion = PRESENTACION_OPCION_START;
    juego->opcion_opciones = OPCIONES_OPCION_MODALIDAD;
    juego->opcion_seleccion_modalidad = juego->config->modo_juego;
    juego->opcion_seleccion_columnas_dx = juego->config->tablero.columnas;
    juego->opcion_seleccion_velocidad = juego->config->velocidad_inicial;
    juego->confirmacion_guardado = 0;
    juego->confirmar_borrar_ranking = 0;
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

    if(juego->temporizador_fijacion){
        gbt_temporizador_destruir(juego->temporizador_fijacion);
        juego->temporizador_fijacion = NULL;
    }

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

    if(juego->ranking){
        ranking_destruir(juego->ranking);
        juego->ranking = NULL;
    }

    free(juego);
}
