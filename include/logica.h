#ifndef LOGICA_H_INCLUDED
#define LOGICA_H_INCLUDED

#include "piezas.h"
#include <stdbool.h>

typedef struct sJuego tJuego;
typedef struct sTablero{
    unsigned char **tablero;
    tTetromino* pieza_actual;
    tTetromino* pieza_siguiente;
}tTablero;

typedef struct{

    uint32_t puntos;
    uint16_t lineas_limpias;
    uint16_t conteo_piezas[11];
    uint16_t piezas_totales;
    uint8_t nivel;
}tEstadoJuego;

/** Funciones tTablero */

/**
 * @desc Crea y reserva memoria para un tablero nuevo.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return tTablero* Puntero al tablero o NULL si error.
 */
tTablero* crear_tablero(tConfigPantalla* config);

/**
 * @desc Libera toda la memoria del tablero y sus piezas.
 * @param tablero tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void vaciar_tablero(tTablero* tablero, tConfigPantalla* config);

/**
 * @desc Limpia el contenido del tablero a cero.
 * @param tablero tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void tableroReiniciar(tTablero* tablero, tConfigPantalla* config);

/** Funciones tEstadoJuego */

/**
 * @desc Crea un nuevo estado de juego inicializado.
 * @return tEstadoJuego* Puntero al estado o NULL si error.
 */
tEstadoJuego* tEstadoCrear();

/**
 * @desc Libera la memoria del estado de juego.
 * @param estado tEstadoJuego* Puntero al estado.
 * @return void
 */
void tEstadoDestruir(tEstadoJuego* estado);


/** Funciones de logica */

/**
 * @desc Desciende la pieza actual un bloque si es valido.
 * @param tablero tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param juego tJuego* Puntero al juego.
 * @return void
 */
void tableroActualizarEstado(tTablero* tablero, tConfigPantalla* config, tJuego* juego);

/**
 * @desc Verifica si la posicion actual de un tetromino es valida.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return bool true si valida, false si colisiona.
 */
bool posicionValida(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Verifica si el tetromino puede descender.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return bool true si puede bajar, false si no.
 */
bool puedeBajar(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Verifica si el tetromino toco el fondo.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return bool true si toco fondo, false si no.
 */
bool piezaTocoFondo(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Verifica si se puede mover a la izquierda.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return bool true si puede mover, false si no.
 */
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Verifica si se puede mover a la derecha.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return bool true si puede mover, false si no.
 */
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Verifica si puede rotar con wall-kick.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param sentido_derecha bool true para derecha.
 * @return int Desplazamiento X o 99 si no puede.
 */
int puedeRotar(tTetromino* tetro, tTablero* t, tConfigPantalla* config, bool sentido_derecha);

/**
 * @desc Fija el tetromino en el tablero.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void piezaAnclar(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Ancla pieza, limpia lineas y genera la siguiente.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param juego tJuego* Puntero al juego.
 * @return void
 */
void piezaAnclarYContinuar(tTablero* t, tConfigPantalla* config, tJuego* juego);

/**
 * @desc Aplica multiplicador por velocidad a puntos base.
 * @param puntos_base uint32_t Puntaje sin multiplicar.
 * @param juego tJuego* Puntero al juego.
 * @return uint32_t Puntaje final.
 */
uint32_t puntosConMultiplicador(uint32_t puntos_base, tJuego* juego);

/**
 * @desc Elimina filas completas y baja las superiores.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param estado tEstadoJuego* Estado de juego.
 * @return int Cantidad de lineas eliminadas.
 */
int lineasLimpiar(tTablero* t, tConfigPantalla* config, tEstadoJuego* estado);

/**
 * @desc Calcula la posicion Y final de la pieza fantasma.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return int Coordenada Y final.
 */
int piezaGhostObtenerY(tTetromino* tetro, tTablero* t, tConfigPantalla* config);

/**
 * @desc Cheat: elimina la linea mas baja con bloques.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param estado tEstadoJuego* Estado de juego.
 * @return int 1 si limpio, 0 si tablero vacio.
 */
int cheatLineaLimpiar(tTablero* t, tConfigPantalla* config, tEstadoJuego* estado);

/**
 * @desc Cheat: re-rollea la pieza actual si cabe.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void cheatReRoll(tTablero* t, tConfigPantalla* config);
#endif // LOGICA_H_INCLUDED
