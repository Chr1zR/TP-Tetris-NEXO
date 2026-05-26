#ifndef DIBUJAR_H_INCLUDED
#define DIBUJAR_H_INCLUDED

#include <stdint.h>
#include <GBT/gbt.h>
#include "config.h"

static inline void pixelDibujar(uint16_t x, uint16_t y, uint8_t color, uint16_t ancho, uint16_t alto) {
    if (x < ancho && y < alto)
        gbt_dibujar_pixel(x, y, color);
}

void lineaHDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color, tConfigPantalla* config);
void lineaVDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color, tConfigPantalla* config);
void rectanguloDibujar(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color, tConfigPantalla* config);
void rectanguloRelleno(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color, tConfigPantalla* config);
void bitmapDibujar(const uint8_t* bitmap, uint16_t x, uint16_t y, uint8_t ancho, uint8_t alto, 
                    uint8_t color, uint8_t escala, tConfigPantalla* config);

#endif // DIBUJAR_H_INCLUDED
