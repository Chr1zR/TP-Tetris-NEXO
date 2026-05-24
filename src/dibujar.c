#include "../include/dibujar.h"

void lineaHDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color) {
    for (uint16_t i = 0; i < longitud; i++)
        pixelDibujar(x + i, y, color);
}

void lineaVDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color) {
    for (uint16_t i = 0; i < longitud; i++)
        pixelDibujar(x, y + i, color);
}

void rectanguloDibujar(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color) {
    lineaHDibujar(x, y, ancho, color);
    lineaHDibujar(x, y + alto - 1, ancho, color);

    lineaVDibujar(x, y, alto, color);
    lineaVDibujar(x + ancho - 1, y, alto, color);
}

void rectanguloRelleno(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color) {
    for (uint16_t i = 0; i < alto; i++)
        lineaHDibujar(x, y + i, ancho, color);
}

void bitmapDibujar(const uint8_t* bitmap, uint16_t x, uint16_t y, uint8_t ancho, uint8_t alto, 
                    uint8_t color, uint8_t escala){

    if (!bitmap)
    return;

    for (uint8_t i = 0; i < alto; i++)
    {
        for (uint8_t j = 0; j < ancho; j++)
        {

            if (bitmap[i * ancho + j] != 0)
            {
                for (uint8_t sy = 0; sy < escala; sy++)
                {
                    for (uint8_t sx = 0; sx < escala; sx++)
                    {
                        pixelDibujar(
                            x + j * escala + sx,
                            y + i * escala + sy,
                            color
                        );
                    }
                }
            }
        }
    }

}