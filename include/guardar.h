#ifndef GUARDAR_H_INCLUDED
#define GUARDAR_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include "../include/juego.h"

#define PARTIDA_ARCHIVO "partida.bin"
#define PARTIDA_MAGIC   0x4E45584F
#define PARTIDA_VERSION 2

typedef struct {
    char nombre[20];
    uint32_t puntos;
    uint16_t nivel;
    uint16_t segundos;
    uint16_t lineas;
} tInfoPartida;

/**
 * @desc Guarda el estado completo de una partida a archivo binario.
 * @param juego tJuego* Puntero al juego.
 * @param ruta const char* Ruta del archivo.
 * @return int 0 si ok, -1 si error.
 */
int  partidaGuardar(tJuego* juego, const char* ruta);

/**
 * @desc Carga el estado completo de una partida desde archivo.
 * @param juego tJuego* Puntero al juego.
 * @param ruta const char* Ruta del archivo.
 * @return int 0 si ok, negativo si error.
 */
int  partidaCargar(tJuego* juego, const char* ruta);

/**
 * @desc Verifica si existe un archivo de partida.
 * @param ruta const char* Ruta del archivo.
 * @return bool true si existe, false si no.
 */
bool partidaExiste(const char* ruta);

/**
 * @desc Lee informacion resumida de una partida guardada.
 * @param ruta const char* Ruta del archivo.
 * @param info tInfoPartida* Puntero a estructura de salida.
 * @return int 0 si ok, -1 o -2 si error.
 */
int  partidaLeerInfo(const char* ruta, tInfoPartida* info);

#endif // GUARDAR_H_INCLUDED
