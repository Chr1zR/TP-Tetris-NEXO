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
#include "../include/guardar.h"

//================ AYUDAS DE NAVEGACION =================

/**
 * @desc Obtiene la siguiente opcion visible del menu segun la modalidad.
 * @param actual uint8_t Opcion actual seleccionada.
 * @param modalidad uint8_t Modalidad de juego activa.
 * @return uint8_t Siguiente opcion visible.
 */
static uint8_t opcionVisibleSiguiente(uint8_t actual, uint8_t modalidad){
    if(modalidad == 1){
        if(actual < OPCIONES_OPCION_VOLVER)
            return actual + 1;
        return actual;
    }
    if(actual == OPCIONES_OPCION_MODALIDAD)
        return OPCIONES_OPCION_VELOCIDAD;
    if(actual == OPCIONES_OPCION_VELOCIDAD)
        return OPCIONES_OPCION_RESOLUCION;
    if(actual == OPCIONES_OPCION_RESOLUCION)
        return OPCIONES_OPCION_CHEATS;
    if(actual == OPCIONES_OPCION_CHEATS)
        return OPCIONES_OPCION_GUARDAR;
    if(actual == OPCIONES_OPCION_GUARDAR)
        return OPCIONES_OPCION_VOLVER;
    return actual;
}

/**
 * @desc Obtiene la opcion visible anterior del menu segun la modalidad.
 * @param actual uint8_t Opcion actual seleccionada.
 * @param modalidad uint8_t Modalidad de juego activa.
 * @return uint8_t Opcion anterior visible.
 */
static uint8_t opcionVisibleAnterior(uint8_t actual, uint8_t modalidad){
    if(modalidad == 1){
        if(actual > OPCIONES_OPCION_MODALIDAD)
            return actual - 1;
        return actual;
    }
    if(actual == OPCIONES_OPCION_VELOCIDAD)
        return OPCIONES_OPCION_MODALIDAD;
    if(actual == OPCIONES_OPCION_RESOLUCION)
        return OPCIONES_OPCION_VELOCIDAD;
    if(actual == OPCIONES_OPCION_CHEATS)
        return OPCIONES_OPCION_RESOLUCION;
    if(actual == OPCIONES_OPCION_GUARDAR)
        return OPCIONES_OPCION_CHEATS;
    if(actual == OPCIONES_OPCION_VOLVER)
        return OPCIONES_OPCION_GUARDAR;
    return actual;
}

//================ AYUDAS DE LOGICA =================

/**
 * @desc Reinicia el temporizador de tolerancia de fijacion de pieza.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void toleranciaReiniciar(tJuego* juego) {
    if(juego->temporizador_fijacion)
        gbt_temporizador_destruir(juego->temporizador_fijacion);
    juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
}

/**
 * @desc Cancela la tolerancia de fijacion de pieza.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void toleranciaCancelar(tJuego* juego) {
    juego->en_tolerancia_fijacion = false;
    if(juego->temporizador_fijacion) {
        gbt_temporizador_destruir(juego->temporizador_fijacion);
        juego->temporizador_fijacion = NULL;
    }
}

/**
 * @desc Guarda el puntaje actual del jugador en el ranking.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void rankingGuardarActual(tJuego* juego) {
    if(juego->ranking && juego->estado->puntos > 0) {
        ranking_agregar(juego->ranking, juego->nombre_jugador,
                       juego->estado->puntos, juego->estado->lineas_limpias);
    }
}

//================ ENTRADAS POR ESTADO =================

/**
 * @desc Procesa la entrada del teclado en la pantalla de presentacion.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputPresentacionProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_ESCAPE) {
        juego->corriendo = 0;
        return;
    }
    if(tecla == GBTK_ARRIBA) {
        if(juego->opcion_presentacion > 0)
            juego->opcion_presentacion--;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ABAJO) {
        if(juego->opcion_presentacion < PRESENTACION_CANT_OPCIONES - 1)
            juego->opcion_presentacion++;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ENTER) {
        switch(juego->opcion_presentacion) {
            case PRESENTACION_OPCION_START:
                juego->estado_pantalla = ESTADO_INGRESAR_NOMBRE;
                juego->cursor_nombre = 0;
                juego->buffer_nombre[0] = '\0';
                juego->frame_cursor = 0;
                juego->cursor_visible = true;
                break;
            case PRESENTACION_OPCION_CARGAR:
                juego->estado_pantalla = ESTADO_CARGAR_PARTIDA;
                break;
            case PRESENTACION_OPCION_PUNTAJES:
                juego->estado_pantalla = ESTADO_PUNTAJES;
                break;
            case PRESENTACION_OPCION_OPCIONES:
                juego->estado_pantalla = ESTADO_MENU_OPCIONES;
                juego->opcion_opciones = OPCIONES_OPCION_MODALIDAD;
                juego->opcion_seleccion_modalidad = juego->config->modo_juego;
                juego->opcion_seleccion_columnas_dx = juego->config->tablero.columnas;
                juego->opcion_seleccion_velocidad = juego->config->velocidad_inicial;
                juego->opcion_seleccion_circular = juego->config->tablero_circular ? 1 : 0;
                juego->opcion_seleccion_extras = juego->config->piezas_extras ? 1 : 0;
                juego->confirmacion_guardado = 0;
                juego->confirmar_borrar_ranking = 0;
                break;
            case PRESENTACION_OPCION_ACERCA_DE:
                juego->estado_pantalla = ESTADO_ACERCA_DE;
                break;
            case PRESENTACION_OPCION_SALIR:
                juego->corriendo = 0;
                return;
        }
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Procesa la entrada del teclado durante el juego.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputJugandoProcesar(tJuego* juego, eGBT_Tecla tecla) {
    uint8_t cols = juego->config->tablero.columnas;

    if(tecla == GBTK_ESCAPE) {
        juego->estado_pantalla = ESTADO_CONFIRMAR_SALIDA;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_p) {
        juego->estado_pantalla = ESTADO_PAUSA;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_DERECHA) {
        if(juego->config->tablero_circular) {
            juego->tablero->pieza_actual->x = ((juego->tablero->pieza_actual->x % cols) + cols) % cols;
        }
        if(puedeMoverDerecha(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
            juego->tablero->pieza_actual->x++;
            if(juego->en_tolerancia_fijacion) {
                if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config))
                    toleranciaReiniciar(juego);
                else
                    toleranciaCancelar(juego);
            }
            juego->redibujar_hud = 1;
        }
        if(juego->temporizador_input_horiz)
            gbt_temporizador_destruir(juego->temporizador_input_horiz);
        juego->temporizador_input_horiz = gbt_temporizador_crear(0.1);
    }
    else if(tecla == GBTK_IZQUIERDA) {
        if(juego->config->tablero_circular) {
            juego->tablero->pieza_actual->x = ((juego->tablero->pieza_actual->x % cols) + cols) % cols;
        }
        if(puedeMoverIzquierda(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
            juego->tablero->pieza_actual->x--;
            if(juego->en_tolerancia_fijacion) {
                if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config))
                    toleranciaReiniciar(juego);
                else
                    toleranciaCancelar(juego);
            }
            juego->redibujar_hud = 1;
        }
        if(juego->temporizador_input_horiz)
            gbt_temporizador_destruir(juego->temporizador_input_horiz);
        juego->temporizador_input_horiz = gbt_temporizador_crear(0.1);
    }
    else if(tecla == GBTK_ARRIBA) {
        int offset_rotacion = puedeRotar(juego->tablero->pieza_actual, juego->tablero, juego->config, true);
        if(offset_rotacion != 99) {
            juego->tablero->pieza_actual->x += offset_rotacion;
            tetrominoRotar(juego->tablero->pieza_actual, true);
            if(juego->en_tolerancia_fijacion) {
                if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config))
                    toleranciaReiniciar(juego);
                else
                    toleranciaCancelar(juego);
            }
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_g && juego->config->cheats_habilitados) {
        juego->ghost_visible = !juego->ghost_visible;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_c && juego->config->cheats_habilitados) {
        if(cheatLineaLimpiar(juego->tablero, juego->config, juego->estado)) {
            juego->estado->puntos += puntosConMultiplicador(100, juego);
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_r && juego->config->cheats_habilitados) {
        cheatReRoll(juego->tablero, juego->config);
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ABAJO && puedeBajar(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
        tetrominoBajar(juego->tablero->pieza_actual);
        juego->estado->puntos += puntosConMultiplicador(1, juego);
        if(juego->en_tolerancia_fijacion)
            toleranciaReiniciar(juego);
        piezaActualDibujar(juego->tablero->pieza_actual, juego->config);
        juego->redibujar_hud = 1;
        if(juego->temporizador_input_vert)
            gbt_temporizador_destruir(juego->temporizador_input_vert);
        juego->temporizador_input_vert = gbt_temporizador_crear(0.08);
    }
    else if(gbt_tecla_sostenida(GBTK_ABAJO) && gbt_temporizador_consumir(juego->temporizador_input_vert) && puedeBajar(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
        tetrominoBajar(juego->tablero->pieza_actual);
        juego->estado->puntos += puntosConMultiplicador(1, juego);
        if(juego->en_tolerancia_fijacion)
            toleranciaReiniciar(juego);
        piezaActualDibujar(juego->tablero->pieza_actual, juego->config);
        juego->redibujar_hud = 1;
    }
    else if(gbt_tecla_sostenida(GBTK_DERECHA) && gbt_temporizador_consumir(juego->temporizador_input_horiz)) {
        if(juego->config->tablero_circular) {
            juego->tablero->pieza_actual->x = ((juego->tablero->pieza_actual->x % cols) + cols) % cols;
        }
        if(puedeMoverDerecha(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
            juego->tablero->pieza_actual->x++;
            if(juego->en_tolerancia_fijacion) {
                if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config))
                    toleranciaReiniciar(juego);
                else
                    toleranciaCancelar(juego);
            }
            juego->redibujar_hud = 1;
        }
    }
    else if(gbt_tecla_sostenida(GBTK_IZQUIERDA) && gbt_temporizador_consumir(juego->temporizador_input_horiz)) {
        if(juego->config->tablero_circular) {
            juego->tablero->pieza_actual->x = ((juego->tablero->pieza_actual->x % cols) + cols) % cols;
        }
        if(puedeMoverIzquierda(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
            juego->tablero->pieza_actual->x--;
            if(juego->en_tolerancia_fijacion) {
                if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config))
                    toleranciaReiniciar(juego);
                else
                    toleranciaCancelar(juego);
            }
            juego->redibujar_hud = 1;
        }
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla de pausa.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputPausaProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_p) {
        juego->estado_pantalla = ESTADO_JUGANDO;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ARRIBA) {
        if(juego->opcion_seleccionada > 0)
            juego->opcion_seleccionada--;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ABAJO) {
        if(juego->opcion_seleccionada < PAUSA_CANT_OPCIONES - 1)
            juego->opcion_seleccionada++;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ENTER) {
        switch(juego->opcion_seleccionada) {
            case PAUSA_OPCION_CONTINUAR:
                juego->estado_pantalla = ESTADO_JUGANDO;
                break;
            case PAUSA_OPCION_REINICIAR:
                juego_reiniciar(juego);
                juego->estado_pantalla = ESTADO_JUGANDO;
                break;
            case PAUSA_OPCION_GUARDAR:
                if(partidaGuardar(juego, PARTIDA_ARCHIVO) == 0)
                    juego->pausa_confirmacion_guardado = 60;
                else
                    juego->pausa_confirmacion_guardado = 255;
                break;
            case PAUSA_OPCION_SALIR:
                juego->corriendo = 0;
                return;
            case PAUSA_OPCION_MENU:
                juego->estado_pantalla = ESTADO_PRESENTACION;
                juego->opcion_seleccionada = PAUSA_OPCION_CONTINUAR;
                break;
            case PAUSA_OPCION_CHEATS:
                juego->estado_pantalla = ESTADO_PAUSA_CHEATS;
                break;
        }
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla de game over.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputGameOverProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_ARRIBA) {
        if(juego->opcion_game_over > 0)
            juego->opcion_game_over--;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ABAJO) {
        if(juego->opcion_game_over < GAME_OVER_CANT_OPCIONES - 1)
            juego->opcion_game_over++;
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ENTER) {
        if(juego->opcion_game_over == GAME_OVER_OPCION_REINICIAR) {
            rankingGuardarActual(juego);
            juego_reiniciar(juego);
            juego->estado_pantalla = ESTADO_JUGANDO;
        }
        else if(juego->opcion_game_over == GAME_OVER_OPCION_MENU) {
            rankingGuardarActual(juego);
            juego_reiniciar(juego);
            juego->estado_pantalla = ESTADO_PRESENTACION;
            juego->opcion_presentacion = PRESENTACION_OPCION_START;
        }
        else if(juego->opcion_game_over == GAME_OVER_OPCION_SALIR) {
            rankingGuardarActual(juego);
            juego->corriendo = 0;
            return;
        }
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ESCAPE) {
        juego->corriendo = 0;
        return;
    }
}

/**
 * @desc Procesa la entrada del teclado para confirmar salida.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputConfirmarSalidaProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_s) {
        rankingGuardarActual(juego);
        juego->corriendo = 0;
        return;
    }
    if(tecla == GBTK_n) {
        juego->estado_pantalla = ESTADO_JUGANDO;
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla de cheats.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputPausaCheatsProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_ESCAPE) {
        juego->estado_pantalla = ESTADO_PAUSA;
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Procesa la entrada del teclado en el menu de opciones.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputMenuOpcionesProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_ESCAPE) {
        juego->estado_pantalla = ESTADO_PRESENTACION;
        juego->redibujar_hud = 1;
        return;
    }
    if(tecla == GBTK_ARRIBA) {
        uint8_t nueva = opcionVisibleAnterior(juego->opcion_opciones, juego->opcion_seleccion_modalidad);
        if(nueva != juego->opcion_opciones) {
            juego->opcion_opciones = nueva;
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_ABAJO) {
        uint8_t nueva = opcionVisibleSiguiente(juego->opcion_opciones, juego->opcion_seleccion_modalidad);
        if(nueva != juego->opcion_opciones) {
            juego->opcion_opciones = nueva;
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_DERECHA) {
        switch(juego->opcion_opciones) {
            case OPCIONES_OPCION_MODALIDAD:
                juego->opcion_seleccion_modalidad = 1 - juego->opcion_seleccion_modalidad;
                if(juego->opcion_seleccion_modalidad == 0) {
                    juego->opcion_seleccion_columnas_dx = 10;
                    juego->opcion_seleccion_circular = 0;
                    juego->opcion_seleccion_extras = 0;
                }
                break;
            case OPCIONES_OPCION_COLUMNAS:
                if(juego->opcion_seleccion_modalidad == 1 && juego->opcion_seleccion_columnas_dx < 16)
                    juego->opcion_seleccion_columnas_dx++;
                break;
            case OPCIONES_OPCION_VELOCIDAD:
                juego->opcion_seleccion_velocidad += 0.1;
                if(juego->opcion_seleccion_velocidad > 2.0)
                    juego->opcion_seleccion_velocidad = 2.0;
                break;
            case OPCIONES_OPCION_CIRCULAR:
                if(juego->opcion_seleccion_modalidad == 1)
                    juego->opcion_seleccion_circular = 1 - juego->opcion_seleccion_circular;
                break;
            case OPCIONES_OPCION_EXTRAS:
                if(juego->opcion_seleccion_modalidad == 1)
                    juego->opcion_seleccion_extras = 1 - juego->opcion_seleccion_extras;
                break;
            case OPCIONES_OPCION_RESOLUCION:
                juego->opcion_seleccion_resolucion = 1 - juego->opcion_seleccion_resolucion;
                break;
            case OPCIONES_OPCION_CHEATS:
                juego->config->cheats_habilitados = !juego->config->cheats_habilitados;
                break;
            default:
                return;
        }
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_IZQUIERDA) {
        switch(juego->opcion_opciones) {
            case OPCIONES_OPCION_MODALIDAD:
                juego->opcion_seleccion_modalidad = 1 - juego->opcion_seleccion_modalidad;
                if(juego->opcion_seleccion_modalidad == 0) {
                    juego->opcion_seleccion_columnas_dx = 10;
                    juego->opcion_seleccion_circular = 0;
                    juego->opcion_seleccion_extras = 0;
                }
                break;
            case OPCIONES_OPCION_COLUMNAS:
                if(juego->opcion_seleccion_modalidad == 1 && juego->opcion_seleccion_columnas_dx > 8)
                    juego->opcion_seleccion_columnas_dx--;
                break;
            case OPCIONES_OPCION_VELOCIDAD:
                juego->opcion_seleccion_velocidad -= 0.1;
                if(juego->opcion_seleccion_velocidad < 0.2)
                    juego->opcion_seleccion_velocidad = 0.2;
                break;
            case OPCIONES_OPCION_CIRCULAR:
                if(juego->opcion_seleccion_modalidad == 1)
                    juego->opcion_seleccion_circular = 1 - juego->opcion_seleccion_circular;
                break;
            case OPCIONES_OPCION_EXTRAS:
                if(juego->opcion_seleccion_modalidad == 1)
                    juego->opcion_seleccion_extras = 1 - juego->opcion_seleccion_extras;
                break;
            case OPCIONES_OPCION_RESOLUCION:
                juego->opcion_seleccion_resolucion = 1 - juego->opcion_seleccion_resolucion;
                break;
            case OPCIONES_OPCION_CHEATS:
                juego->config->cheats_habilitados = !juego->config->cheats_habilitados;
                break;
            default:
                return;
        }
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ENTER) {
        if(juego->opcion_opciones == OPCIONES_OPCION_GUARDAR) {
            juego->config->modo_juego = (tModoJuego)juego->opcion_seleccion_modalidad;
            juego->config->tablero.columnas = juego->opcion_seleccion_columnas_dx;

            uint16_t nuevo_ancho = juego->opcion_seleccion_resolucion == 0 ? 640 : 320;
            uint16_t nuevo_alto = juego->opcion_seleccion_resolucion == 0 ? 480 : 200;
            bool resolucion_cambio = (juego->config->ventana.ancho != nuevo_ancho ||
                                      juego->config->ventana.alto != nuevo_alto);

            juego->config->ventana.ancho = nuevo_ancho;
            juego->config->ventana.alto = nuevo_alto;

            configGeometriaRecalcular(juego->config);

            juego->config->velocidad_inicial = juego->opcion_seleccion_velocidad;
            juego->config->tablero_circular = juego->opcion_seleccion_circular != 0;
            juego->config->piezas_extras = juego->opcion_seleccion_extras != 0;

            if(resolucion_cambio) {
                reiniciar_plataforma(juego->config);
                juego->redibujar_hud = 1;
            }

            if(configGuardar(juego->config, "tetris.cfg") == 0)
                juego->confirmacion_guardado = 90;
            else
                juego->confirmacion_guardado = 255;
            juego->redibujar_hud = 1;
        }
        else if(juego->opcion_opciones == OPCIONES_OPCION_VOLVER) {
            juego->estado_pantalla = ESTADO_PRESENTACION;
            juego->redibujar_hud = 1;
        }
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla acerca de.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputAcercaDeProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_ESCAPE) {
        juego->estado_pantalla = ESTADO_PRESENTACION;
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla de cargar partida.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputCargarPartidaProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(tecla == GBTK_ESCAPE) {
        juego->estado_pantalla = ESTADO_PRESENTACION;
        juego->cargar_mensaje[0] = '\0';
        juego->redibujar_hud = 1;
    }
    else if(tecla == GBTK_ENTER) {
        if(!partidaExiste(PARTIDA_ARCHIVO)) {
            strcpy(juego->cargar_mensaje, "No existe partida guardada");
            juego->redibujar_hud = 1;
        } else {
            int res = partidaCargar(juego, PARTIDA_ARCHIVO);
            if(res == 0) {
                juego->cargar_mensaje[0] = '\0';
                juego->estado_pantalla = ESTADO_JUGANDO;
                juego->redibujar_hud = 1;
            } else if(res == -4) {
                strcpy(juego->cargar_mensaje, "Error al recrear ventana");
                juego->redibujar_hud = 1;
            } else if(res == -5) {
                strcpy(juego->cargar_mensaje, "Error al recrear tablero");
                juego->redibujar_hud = 1;
            } else {
                strcpy(juego->cargar_mensaje, "Archivo corrupto");
                juego->redibujar_hud = 1;
            }
        }
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla de puntajes.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputPuntajesProcesar(tJuego* juego, eGBT_Tecla tecla) {
    if(juego->confirmar_borrar_ranking) {
        if(tecla == GBTK_s) {
            if(juego->ranking)
                ranking_borrar(juego->ranking);
            juego->confirmar_borrar_ranking = 0;
            juego->redibujar_hud = 1;
        }
        else if(tecla == GBTK_n || tecla == GBTK_ESCAPE) {
            juego->confirmar_borrar_ranking = 0;
            juego->redibujar_hud = 1;
        }
    } else {
        if(tecla == GBTK_ESCAPE) {
            juego->estado_pantalla = ESTADO_PRESENTACION;
            juego->redibujar_hud = 1;
        }
        else if(tecla == GBTK_r) {
            if(juego->ranking && ranking_tamanio(juego->ranking) > 0) {
                juego->confirmar_borrar_ranking = 1;
                juego->redibujar_hud = 1;
            }
        }
    }
}

/**
 * @desc Procesa la entrada del teclado en la pantalla de ingresar nombre.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputIngresarNombreProcesar(tJuego* juego, eGBT_Tecla tecla) {
    juego->frame_cursor++;
    if(juego->frame_cursor % 30 == 0)
        juego->cursor_visible = !juego->cursor_visible;

    if(tecla == GBTK_ENTER) {
        if(juego->cursor_nombre > 0) {
            juego->buffer_nombre[juego->cursor_nombre] = '\0';
            strcpy(juego->nombre_jugador, juego->buffer_nombre);
            juego_reiniciar(juego);
            juego->estado_pantalla = ESTADO_JUGANDO;
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_RETROCESO) {
        if(juego->cursor_nombre > 0) {
            juego->cursor_nombre--;
            juego->buffer_nombre[juego->cursor_nombre] = '\0';
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla >= GBTK_a && tecla <= GBTK_z) {
        if(juego->cursor_nombre < 19) {
            char c = (char)('A' + (tecla - GBTK_a));
            juego->buffer_nombre[juego->cursor_nombre] = c;
            juego->cursor_nombre++;
            juego->buffer_nombre[juego->cursor_nombre] = '\0';
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla >= GBTK_0 && tecla <= GBTK_9) {
        if(juego->cursor_nombre < 19) {
            char c = (char)('0' + (tecla - GBTK_0));
            juego->buffer_nombre[juego->cursor_nombre] = c;
            juego->cursor_nombre++;
            juego->buffer_nombre[juego->cursor_nombre] = '\0';
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_ESPACIO) {
        if(juego->cursor_nombre < 19) {
            juego->buffer_nombre[juego->cursor_nombre] = ' ';
            juego->cursor_nombre++;
            juego->buffer_nombre[juego->cursor_nombre] = '\0';
            juego->redibujar_hud = 1;
        }
    }
    else if(tecla == GBTK_ESCAPE) {
        if(juego->cursor_nombre > 0) {
            strcpy(juego->buffer_nombre, "JUGADOR1");
            juego->cursor_nombre = 8;
            juego->buffer_nombre[juego->cursor_nombre] = '\0';
            strcpy(juego->nombre_jugador, juego->buffer_nombre);
            juego->estado_pantalla = ESTADO_JUGANDO;
        } else {
            juego->estado_pantalla = ESTADO_PRESENTACION;
            juego->opcion_presentacion = PRESENTACION_OPCION_START;
        }
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Delega el procesamiento de entrada segun el estado actual de pantalla.
 * @param juego tJuego* Puntero al estado del juego.
 * @param tecla eGBT_Tecla Tecla presionada.
 * @return void Sin retorno.
 */
static void inputProcesar(tJuego* juego, eGBT_Tecla tecla) {
    switch(juego->estado_pantalla) {
        case ESTADO_PRESENTACION:
            inputPresentacionProcesar(juego, tecla);
            break;
        case ESTADO_JUGANDO:
            inputJugandoProcesar(juego, tecla);
            break;
        case ESTADO_PAUSA:
            inputPausaProcesar(juego, tecla);
            break;
        case ESTADO_PAUSA_CHEATS:
            inputPausaCheatsProcesar(juego, tecla);
            break;
        case ESTADO_GAME_OVER:
            inputGameOverProcesar(juego, tecla);
            break;
        case ESTADO_CONFIRMAR_SALIDA:
            inputConfirmarSalidaProcesar(juego, tecla);
            break;
        case ESTADO_MENU_OPCIONES:
            inputMenuOpcionesProcesar(juego, tecla);
            break;
        case ESTADO_ACERCA_DE:
            inputAcercaDeProcesar(juego, tecla);
            break;
        case ESTADO_CARGAR_PARTIDA:
            inputCargarPartidaProcesar(juego, tecla);
            break;
        case ESTADO_PUNTAJES:
            inputPuntajesProcesar(juego, tecla);
            break;
        case ESTADO_INGRESAR_NOMBRE:
            inputIngresarNombreProcesar(juego, tecla);
            break;
    }
}

//================ UPDATE POR ESTADO =================

/**
 * @desc Actualiza temporizadores de confirmacion segun el estado de pantalla.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void updateProcesar(tJuego* juego) {
    if(juego->estado_pantalla == ESTADO_PAUSA) {
        if(juego->pausa_confirmacion_guardado > 0 && juego->pausa_confirmacion_guardado < 255)
            juego->pausa_confirmacion_guardado--;
    }
    else if(juego->estado_pantalla == ESTADO_MENU_OPCIONES) {
        if(juego->confirmacion_guardado > 0 && juego->confirmacion_guardado < 255)
            juego->confirmacion_guardado--;
    }
}

//================ RENDER POR ESTADO =================

/**
 * @desc Renderiza la pantalla de juego activo.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void juegoRenderizar(tJuego* juego) {
    if(juego->redibujar_hud) {
        fondoHudDibujar(juego->config);
        hudRecuadrosDibujar(juego->config);
        hudTituloNeonDibujar(juego->config);
        hudNextDibujar(juego->tablero->pieza_siguiente, juego->config);
        hudPiezasDibujar(juego->estado->conteo_piezas, juego->config);

        bool cga = juego->config->ventana.ancho < 400;
        uint8_t escala_val = cga ? 1 : 2;
        uint8_t escala_nivel = cga ? 1 : 3;
        uint8_t escala_nombre = cga ? 1 : 3;

        hudScoreDibujar(juego->estado->puntos, escala_val, juego->config);
        hudNivelDibujar(juego->estado->nivel, escala_nivel, juego->config);
        hudLineasDibujar(juego->estado->lineas_limpias, escala_val, juego->config);

        uint16_t x_nombre = juego->config->hud.disposicion.nombre.offset_x_inicial + (cga ? 3 : 5);
        uint16_t y_nombre = juego->config->hud.disposicion.nombre.offset_y_inicial + (cga ? 4 : 8);
        hudPalabraDibujar(juego->nombre_jugador, x_nombre, y_nombre, CC, escala_nombre, juego->config);

        cuadriculaDibujar(juego->tablero, juego->config);
        piezasAncladasDibujar(juego->tablero, juego->config);
        hudTiempoDibujar(juego->seg_transcurrido, escala_val, juego->config);
    }
    if(juego->config->cheats_habilitados && juego->ghost_visible) {
        int ghost_y = piezaGhostObtenerY(juego->tablero->pieza_actual, juego->tablero, juego->config);
        piezaGhostDibujar(juego->tablero->pieza_actual, ghost_y, juego->config);
    }
    piezaActualDibujar(juego->tablero->pieza_actual, juego->config);

    if(gbt_temporizador_consumir(juego->temporizador)) {
        tableroActualizarEstado(juego->tablero, juego->config, juego);
        if(piezaTocoFondo(juego->tablero->pieza_actual, juego->tablero, juego->config)) {
            juego->en_tolerancia_fijacion = true;
            juego->intervalo_fijacion = juego->intervalo_actual * 0.5;
            if(juego->temporizador_fijacion)
                gbt_temporizador_destruir(juego->temporizador_fijacion);
            juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
        }
        juego->redibujar_hud = 1;
    }
    if(juego->en_tolerancia_fijacion && gbt_temporizador_consumir(juego->temporizador_fijacion)) {
        piezaAnclarYContinuar(juego->tablero, juego->config, juego);
        juego->en_tolerancia_fijacion = false;
        juego->redibujar_hud = 1;
    }
    if(gbt_temporizador_consumir(juego->temporizador_seg)) {
        juego->seg_transcurrido++;
        juego->redibujar_hud = 1;
    }
    if(juego->game_over) {
        juego->estado_pantalla = ESTADO_GAME_OVER;
        juego->redibujar_hud = 1;
    }
}

/**
 * @desc Renderiza la pantalla de presentacion.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void presentacionRenderizar(tJuego* juego) {
    presentarPantallaDibujar(juego->config, juego->opcion_presentacion);
}

/**
 * @desc Renderiza la pantalla de pausa.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void pausaRenderizar(tJuego* juego) {
    cubrirPausaDibujar(juego->config, juego->opcion_seleccionada, juego->pausa_confirmacion_guardado);
}

/**
 * @desc Renderiza la pantalla de cheats en pausa.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void pausaCheatsRenderizar(tJuego* juego) {
    pausaCheatsDibujar(juego->config);
}

/**
 * @desc Renderiza la pantalla de game over.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void gameOverRenderizar(tJuego* juego) {
    cubrirGameOverDibujar(juego->config, juego->estado->puntos, juego->estado->lineas_limpias, juego->nombre_jugador, juego->opcion_game_over);
}

/**
 * @desc Renderiza la pantalla de confirmar salida.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void confirmarSalidaRenderizar(tJuego* juego) {
    cubrirConfirmarSalidaDibujar(juego->config);
}

/**
 * @desc Renderiza el menu de opciones.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void menuOpcionesRenderizar(tJuego* juego) {
    menuConfiguracionDibujar(juego->config, juego->opcion_opciones,
        juego->opcion_seleccion_modalidad,
        juego->opcion_seleccion_columnas_dx,
        juego->opcion_seleccion_velocidad,
        juego->opcion_seleccion_circular,
        juego->opcion_seleccion_extras,
        juego->opcion_seleccion_resolucion,
        juego->confirmacion_guardado,
        juego->config->cheats_habilitados);
}

/**
 * @desc Renderiza la pantalla acerca de.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void acercaDeRenderizar(tJuego* juego) {
    pantallaAcercaDeDibujar(juego->config);
}

/**
 * @desc Renderiza la pantalla de cargar partida.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void cargarPartidaRenderizar(tJuego* juego) {
    bool hay = partidaExiste(PARTIDA_ARCHIVO);
    tInfoPartida info = {0};
    if(hay)
        partidaLeerInfo(PARTIDA_ARCHIVO, &info);
    pantallaCargarPartidaDibujar(juego->config, hay, info.nombre, info.puntos,
                                 info.nivel, info.lineas, info.segundos, juego->cargar_mensaje);
}

/**
 * @desc Renderiza la pantalla de puntajes.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void puntajesRenderizar(tJuego* juego) {
    pantallaPuntajesDibujar(juego->config, juego->ranking, juego->confirmar_borrar_ranking);
}

/**
 * @desc Renderiza la pantalla de ingresar nombre.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void ingresarNombreRenderizar(tJuego* juego) {
    pantallaIngresarNombreDibujar(juego->config, juego->buffer_nombre,
        juego->cursor_nombre, juego->cursor_visible);
}

/**
 * @desc Delega el renderizado segun el estado actual de pantalla.
 * @param juego tJuego* Puntero al estado del juego.
 * @return void Sin retorno.
 */
static void renderizarPantalla(tJuego* juego) {
    switch(juego->estado_pantalla) {
        case ESTADO_PRESENTACION:
            presentacionRenderizar(juego);
            break;
        case ESTADO_JUGANDO:
            juegoRenderizar(juego);
            break;
        case ESTADO_PAUSA:
            pausaRenderizar(juego);
            break;
        case ESTADO_PAUSA_CHEATS:
            pausaCheatsRenderizar(juego);
            break;
        case ESTADO_GAME_OVER:
            gameOverRenderizar(juego);
            break;
        case ESTADO_CONFIRMAR_SALIDA:
            confirmarSalidaRenderizar(juego);
            break;
        case ESTADO_MENU_OPCIONES:
            menuOpcionesRenderizar(juego);
            break;
        case ESTADO_ACERCA_DE:
            acercaDeRenderizar(juego);
            break;
        case ESTADO_CARGAR_PARTIDA:
            cargarPartidaRenderizar(juego);
            break;
        case ESTADO_PUNTAJES:
            puntajesRenderizar(juego);
            break;
        case ESTADO_INGRESAR_NOMBRE:
            ingresarNombreRenderizar(juego);
            break;
    }
}

//================ BUCLE PRINCIPAL =================

int iniciar_juego(int argc, char* argv[]){
    tJuego* juego = juego_crear(argc, argv);
    if(!juego){
        puts("Error al iniciar juego.");
        return ERROR_INICIALIZACION;
    }
    eGBT_Tecla tecla;

    while(juego->corriendo){
        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();
        inputProcesar(juego, tecla);
        updateProcesar(juego);

        gbt_borrar_backbuffer(N);
        renderizarPantalla(juego);
        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    juego_destruir(juego);
    printf("TODO OK.\n");
    return TODO_OK;
}

//================ CREAR / REINICIAR / DESTRUIR =================

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

    bool argv_tiene_resolucion = false;
    uint16_t argv_ancho = 0, argv_alto = 0;
    uint8_t argv_escala = 0;

    if(argc == 3 && argv && argv[1] && argv[2]) {
        int escala = atoi(argv[2]);
        if(escala > 0) {
            if(strcmp(argv[1], "VGA") == 0) {
                argv_ancho = 640;
                argv_alto = 480;
                argv_escala = (uint8_t)escala;
                argv_tiene_resolucion = true;
            } else if(strcmp(argv[1], "CGA") == 0) {
                argv_ancho = 320;
                argv_alto = 200;
                argv_escala = (uint8_t)escala;
                argv_tiene_resolucion = true;
            }
        }
    }

    if(configCargar(juego->config, "tetris.cfg") != 0){
        free(juego->config);
        juego->config = NULL;
        juego->config = crear_config(argc, argv);
        if(!juego->config){
            juego_destruir(juego);
            return NULL;
        }
    } else if(argv_tiene_resolucion) {
        juego->config->ventana.ancho = argv_ancho;
        juego->config->ventana.alto = argv_alto;
        juego->config->ventana.escala = argv_escala;
        configGeometriaRecalcular(juego->config);
    }

    if(!juego->config){
        juego_destruir(juego);
        return NULL;
    }

    //================ PLATAFORMA =================
    char nombreVentana[128];
    if(!iniciar_plataforma(nombreVentana, sizeof(nombreVentana), juego->config)){
        juego_destruir(juego);
        return NULL;
    }

    //================ TEMPORIZADORES =================
    juego->temporizador = gbt_temporizador_crear(juego->config->velocidad_inicial);
    if(!juego->temporizador){
        juego_destruir(juego);
        return NULL;
    }

    juego->intervalo_actual = juego->config->velocidad_inicial;
    juego->temporizador_fijacion = NULL;
    juego->en_tolerancia_fijacion = false;
    juego->intervalo_fijacion = juego->config->velocidad_inicial * 0.5;

    juego->temporizador_seg = gbt_temporizador_crear(1);
    if(!juego->temporizador_seg){
        juego_destruir(juego);
        return NULL;
    }

    juego->temporizador_input_horiz = gbt_temporizador_crear(0.1);
    if(!juego->temporizador_input_horiz){
        juego_destruir(juego);
        return NULL;
    }

    juego->temporizador_input_vert = gbt_temporizador_crear(0.08);
    if(!juego->temporizador_input_vert){
        juego_destruir(juego);
        return NULL;
    }

    //================ TABLERO =================
    juego->tablero = crear_tablero(juego->config);
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

    tetrominoAleatorio(juego->tablero->pieza_actual,
                       juego->config->tablero.columnas,
                       juego->config->piezas_extras);
    tetrominoAleatorio(juego->tablero->pieza_siguiente,
                       juego->config->tablero.columnas,
                       juego->config->piezas_extras);

    juego->corriendo = 1;
    juego->game_over = false;
    juego->seg_transcurrido = 0;
    juego->redibujar_hud = 1;
    juego->estado_pantalla = ESTADO_PRESENTACION;
    juego->opcion_seleccionada = PAUSA_OPCION_CONTINUAR;
    juego->opcion_game_over = GAME_OVER_OPCION_REINICIAR;
    juego->opcion_presentacion = PRESENTACION_OPCION_START;
    juego->opcion_seleccion_resolucion = juego->config->ventana.ancho >= 640 ? 0 : 1;
    juego->pausa_confirmacion_guardado = 0;
    juego->cargar_mensaje[0] = '\0';
    juego->ghost_visible = true;

    strcpy(juego->nombre_jugador, "JUGADOR1");

    juego->ranking = ranking_crear(10);
    if(juego->ranking)
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
    for(int i = 0; i < 11; i++)
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
    if(juego->temporizador_input_horiz){
        gbt_temporizador_destruir(juego->temporizador_input_horiz);
    }
    juego->temporizador_input_horiz = gbt_temporizador_crear(0.1);
    if(juego->temporizador_input_vert){
        gbt_temporizador_destruir(juego->temporizador_input_vert);
    }
    juego->temporizador_input_vert = gbt_temporizador_crear(0.08);

    //================ RESET PIEZAS =================
    tTetromino_Destruir(juego->tablero->pieza_actual);
    tTetromino_Destruir(juego->tablero->pieza_siguiente);
    juego->tablero->pieza_actual = tetrominoCrear();
    juego->tablero->pieza_siguiente = tetrominoCrear();
    tetrominoAleatorio(juego->tablero->pieza_actual,
                       juego->config->tablero.columnas,
                       juego->config->piezas_extras);
    tetrominoAleatorio(juego->tablero->pieza_siguiente,
                       juego->config->tablero.columnas,
                       juego->config->piezas_extras);

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
    juego->opcion_seleccion_circular = juego->config->tablero_circular ? 1 : 0;
    juego->opcion_seleccion_extras = juego->config->piezas_extras ? 1 : 0;
    juego->opcion_seleccion_resolucion = juego->config->ventana.ancho >= 640 ? 0 : 1;
    juego->confirmacion_guardado = 0;
    juego->confirmar_borrar_ranking = 0;
    juego->pausa_confirmacion_guardado = 0;
    juego->cargar_mensaje[0] = '\0';
    juego->ghost_visible = true;
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
    if(juego->temporizador_input_horiz){
        gbt_temporizador_destruir(juego->temporizador_input_horiz);
        juego->temporizador_input_horiz = NULL;
    }
    if(juego->temporizador_input_vert){
        gbt_temporizador_destruir(juego->temporizador_input_vert);
        juego->temporizador_input_vert = NULL;
    }

    parar_plataforma(juego->temporizador, juego->temporizador_seg);
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
