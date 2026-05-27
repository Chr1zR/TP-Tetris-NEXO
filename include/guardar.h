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

int  partidaGuardar(tJuego* juego, const char* ruta);
int  partidaCargar(tJuego* juego, const char* ruta);
bool partidaExiste(const char* ruta);
int  partidaLeerInfo(const char* ruta, tInfoPartida* info);

#endif // GUARDAR_H_INCLUDED
