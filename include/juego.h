#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <GBT/gbt.h>
#include "../include/config.h"
#include "../include/logica.h"
#include "../include/ranking.h"

typedef enum {
    ESTADO_PRESENTACION,
    ESTADO_JUGANDO,
    ESTADO_PAUSA,
    ESTADO_GAME_OVER,
    ESTADO_CONFIRMAR_SALIDA,
    ESTADO_MENU_OPCIONES,
    ESTADO_ACERCA_DE,
    ESTADO_CARGAR_PARTIDA,
    ESTADO_PUNTAJES,
    ESTADO_INGRESAR_NOMBRE,
    ESTADO_PAUSA_CHEATS
} tEstadoPantalla;

#define PRESENTACION_OPCION_START 0
#define PRESENTACION_OPCION_CARGAR 1
#define PRESENTACION_OPCION_PUNTAJES 2
#define PRESENTACION_OPCION_OPCIONES 3
#define PRESENTACION_OPCION_ACERCA_DE 4
#define PRESENTACION_OPCION_SALIR 5
#define PRESENTACION_CANT_OPCIONES 6

#define PAUSA_OPCION_CONTINUAR 0
#define PAUSA_OPCION_REINICIAR 1
#define PAUSA_OPCION_GUARDAR 2
#define PAUSA_OPCION_MENU 3
#define PAUSA_OPCION_CHEATS 4
#define PAUSA_OPCION_SALIR 5
#define PAUSA_CANT_OPCIONES 6

#define GAME_OVER_OPCION_REINICIAR 0
#define GAME_OVER_OPCION_SALIR 1
#define GAME_OVER_CANT_OPCIONES 2

#define OPCIONES_OPCION_MODALIDAD 0
#define OPCIONES_OPCION_COLUMNAS 1
#define OPCIONES_OPCION_CIRCULAR 2
#define OPCIONES_OPCION_EXTRAS 3
#define OPCIONES_OPCION_VELOCIDAD 4
#define OPCIONES_OPCION_RESOLUCION 5
#define OPCIONES_OPCION_CHEATS 6
#define OPCIONES_OPCION_GUARDAR 7
#define OPCIONES_OPCION_VOLVER 8
#define OPCIONES_CANT_OPCIONES 9

typedef struct sJuego {

    tConfigPantalla* config;

    tGBT_Temporizador* temporizador;
    tGBT_Temporizador* temporizador_seg;
    tGBT_Temporizador* temporizador_input_horiz;
    tGBT_Temporizador* temporizador_input_vert;

    tTablero* tablero;
    tEstadoJuego* estado;

    uint8_t corriendo;
    bool game_over;
    tEstadoPantalla estado_pantalla;
    uint8_t opcion_seleccionada;
    uint8_t opcion_game_over;
    uint8_t opcion_presentacion;

    uint16_t seg_transcurrido;

    char nombre_jugador[20];

    tRanking* ranking;

    uint8_t redibujar_hud;

    double intervalo_actual;

    bool en_tolerancia_fijacion;
    double intervalo_fijacion;
    tGBT_Temporizador* temporizador_fijacion;
    bool ghost_visible;

    char buffer_nombre[20];
    uint8_t cursor_nombre;
    uint8_t frame_cursor;
    bool cursor_visible;

    uint8_t opcion_opciones;
    uint8_t opcion_seleccion_modalidad;
    uint8_t opcion_seleccion_columnas_dx;
    double opcion_seleccion_velocidad;
    uint8_t opcion_seleccion_circular;
    uint8_t opcion_seleccion_extras;
    uint8_t opcion_seleccion_resolucion;
    uint8_t confirmacion_guardado;
    uint8_t confirmar_borrar_ranking;

    uint8_t pausa_confirmacion_guardado;
    char cargar_mensaje[64];

} tJuego;


tJuego* juego_crear(int argc, char* argv[]);
int iniciar_juego(int argc, char* argv[]);
void juego_reiniciar(tJuego* juego);
void juego_destruir(tJuego* juego);

#endif // JUEGO_H_INCLUDED
