#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <stdint.h>

typedef struct{

    uint16_t ancho;
    uint16_t alto;
    uint8_t escala;
}tVentana;

typedef struct {
    uint8_t columnas;
    uint8_t filas;
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
    //Ventana fisica
    uint16_t ancho_ventana; //CGA = 320x200
    uint16_t alto_ventana;  //VGA = 640x480(default)
    uint8_t escala;

    //Variables a calcular
    uint16_t ancho_tablero;
    uint16_t alto_tablero;
    uint16_t tam_bloque;
    uint16_t ancho_hud_izq;
    uint16_t ancho_hud_der;
    uint16_t offset_tablero_x;
    uint16_t offset_tablero_y;

    uint8_t columnas;
    uint8_t filas;

    tDisposicionHud hud;

    //tVentana ventana;
    //tDisposicionTablero tablero;
    //tHUD hud;
}tConfigPantalla;


//static void tConfigInicializar(tConfigPantalla* config);
tConfigPantalla* crear_config_default();
tConfigPantalla* crear_config(char* argv[]);
void configDestruir(tConfigPantalla* config);
void anchosCalcular(tConfigPantalla* config);
uint8_t modalidadObtener();
uint8_t columnasDeluxeObtener();


#endif // CONFIG_H_INCLUDED
