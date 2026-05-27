#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "paleta.h"
#include "config.h"
#include "piezas.h"
#include "ranking.h"

#define PRESENTACION_CANT_OPCIONES 6

#define ALTO_LETRA 5
#define ALTO_NUMERO 7

void fondoHudDibujar(tConfigPantalla* config);

void disposicionHudCalcular(tConfigPantalla* config);

void hudRecuadrosDibujar(tConfigPantalla* config);
void hudPalabraDibujar(const char* palabra, uint16_t pos_x, uint16_t pos_y, uint8_t color, uint8_t escala, tConfigPantalla* config);
void hudScoreDibujar(uint32_t score, uint8_t escala, tConfigPantalla* config);
void hudNivelDibujar(uint8_t nivel, uint8_t escala, tConfigPantalla* config);
void hudLineasDibujar(uint16_t lineas, uint8_t escala, tConfigPantalla* config);
void hudTiempoDibujar(uint16_t tiempo_segundos, uint8_t escala, tConfigPantalla* config);
void hudNextDibujar(tTetromino* siguiente, tConfigPantalla* config);
void hudPiezasDibujar(uint16_t conteo[11], tConfigPantalla* config);
void hudTituloNeonDibujar(tConfigPantalla* config);
void cubrirPausaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada, uint8_t confirmacion_guardado);
void cubrirGameOverDibujar(tConfigPantalla* config, uint32_t score, uint16_t lineas, const char* nombre, uint8_t opcion_seleccionada);
void cubrirConfirmarSalidaDibujar(tConfigPantalla* config);
void presentarPantallaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada);
void menuConfiguracionDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada, uint8_t modalidad, uint8_t columnas_dx, double velocidad, uint8_t circular, uint8_t extras, uint8_t resolucion, uint8_t confirmacion_guardado, bool cheats);
void pausaCheatsDibujar(tConfigPantalla* config);
void pantallaAcercaDeDibujar(tConfigPantalla* config);
void pantallaCargarPartidaDibujar(tConfigPantalla* config, bool hay_partida,
                                 const char* nombre, uint32_t puntos,
                                 uint16_t nivel, uint16_t lineas, uint16_t segundos,
                                 const char* mensaje_error);
void pantallaPuntajesDibujar(tConfigPantalla* config, tRanking* rank, uint8_t confirmar_borrar);
void pantallaIngresarNombreDibujar(tConfigPantalla* config, const char* buffer, uint8_t cursor_pos, bool cursor_visible);
void pantallaMensajeDibujar(tConfigPantalla* config, const char* titulo, const char* mensaje);

#endif // HUD_H_INCLUDED