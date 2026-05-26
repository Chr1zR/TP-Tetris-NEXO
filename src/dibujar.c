#include "../include/dibujar.h"

void lineaHDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color, tConfigPantalla* config) {
    uint16_t limite_ancho = config->ventana.ancho;
    uint16_t limite_alto = config->ventana.alto;
    for (uint16_t i = 0; i < longitud; i++)
        pixelDibujar(x + i, y, color, limite_ancho, limite_alto);
}

void lineaVDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color, tConfigPantalla* config) {
    uint16_t limite_ancho = config->ventana.ancho;
    uint16_t limite_alto = config->ventana.alto;
    for (uint16_t i = 0; i < longitud; i++)
        pixelDibujar(x, y + i, color, limite_ancho, limite_alto);
}

void rectanguloDibujar(uint16_t x, uint16_t y, uint16_t ancho_rect, uint16_t alto_rect, uint8_t color, tConfigPantalla* config) {
    lineaHDibujar(x, y, ancho_rect, color, config);
    lineaHDibujar(x, y + alto_rect - 1, ancho_rect, color, config);
    lineaVDibujar(x, y, alto_rect, color, config);
    lineaVDibujar(x + ancho_rect - 1, y, alto_rect, color, config);
}

void rectanguloRelleno(uint16_t x, uint16_t y, uint16_t ancho_rect, uint16_t alto_rect, uint8_t color, tConfigPantalla* config) {
    for (uint16_t i = 0; i < alto_rect; i++)
        lineaHDibujar(x, y + i, ancho_rect, color, config);
}

void bitmapDibujar(const uint8_t* bitmap, uint16_t x, uint16_t y, uint8_t ancho, uint8_t alto, 
                    uint8_t color, uint8_t escala, tConfigPantalla* config){

    if (!bitmap || !config)
        return;

    uint16_t limite_ancho = config->ventana.ancho;
    uint16_t limite_alto = config->ventana.alto;

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
                            color,
                            limite_ancho,
                            limite_alto
                        );
                    }
                }
            }
        }
    }

}
