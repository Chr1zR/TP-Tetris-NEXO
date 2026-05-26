#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <GBT/gbt.h>
#include "../include/config.h"
#include "../include/logica.h"

typedef enum {
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAME_OVER,
    ESTADO_CONFIRMAR_SALIDA
} tEstadoPantalla;

#define PAUSA_OPCION_CONTINUAR 0
#define PAUSA_OPCION_REINICIAR 1
#define PAUSA_OPCION_SALIR 2
#define PAUSA_CANT_OPCIONES 3

#define GAME_OVER_OPCION_REINICIAR 0
#define GAME_OVER_OPCION_SALIR 1
#define GAME_OVER_CANT_OPCIONES 2

typedef struct sJuego {

    tConfigPantalla* config;

    tGBT_Temporizador* temporizador;
    tGBT_Temporizador* temporizador_seg;

    tTablero* tablero;
    tEstadoJuego* estado;

    uint8_t corriendo;
    bool game_over;
    tEstadoPantalla estado_pantalla;
    uint8_t opcion_seleccionada;
    uint8_t opcion_game_over;

    uint16_t seg_transcurrido;

    char nombre_jugador[20];

    uint8_t redibujar_hud;

    double intervalo_actual;

} tJuego;


tJuego* juego_crear(int argc, char* argv[]);
int iniciar_juego(int argc, char* argv[]);
void juego_reiniciar(tJuego* juego);
void juego_destruir(tJuego* juego);

#endif // JUEGO_H_INCLUDED
