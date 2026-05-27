#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdint.h>

#include <stdbool.h>

#define FILAS_INVISIBLES 2

typedef enum {
    MODO_CLASICO,
    MODO_DX
} tModoJuego;

typedef struct{

    uint16_t ancho;
    uint16_t alto;
    uint8_t escala;
}tVentana;

typedef struct {
    uint8_t columnas;
    uint8_t filas;           // total: visibles + invisibles
    uint8_t filas_visibles;  // solo las visibles en pantalla
    uint16_t tam_bloque;

    uint16_t ancho_px;
    uint16_t alto_px;

    uint16_t offset_x;
    uint16_t offset_y;
} tDisposicionTablero;

typedef struct {

    uint16_t offset_x_inicial;
    uint16_t offset_y_inicial;
    uint16_t offset_x_final;
    uint16_t offset_y_final;
}tPosHud;

typedef struct{
    tPosHud next;
    tPosHud score;
    tPosHud nivel;
    tPosHud lineas;
    tPosHud nombre;
    tPosHud piezas;
    tPosHud tiempo;
}tDisposicionHud;

typedef struct {
    uint16_t ancho_izq;
    uint16_t ancho_der;
    tDisposicionHud disposicion;     // Coordenadas puntos, pieza siguiente, nivel, etc.
} tHUD;

typedef struct {

    tVentana ventana;
    tDisposicionTablero tablero;
    tHUD hud;
    tModoJuego modo_juego;
    double velocidad_inicial;
    bool tablero_circular;
    bool piezas_extras;
    bool cheats_habilitados;
}tConfigPantalla;


tConfigPantalla* crear_config(int argc, char* argv[]);
void configDestruir(tConfigPantalla* config);
void configGeometriaRecalcular(tConfigPantalla* config);
void anchosCalcular(tConfigPantalla* config);
int configGuardar(tConfigPantalla* config, const char* ruta);
int configCargar(tConfigPantalla* config, const char* ruta);


#endif // CONFIG_H_INCLUDED
