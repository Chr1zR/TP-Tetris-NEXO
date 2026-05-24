#include<GBT/gbt.h>

 #define WIDTH 640
 #define HEIGHT 480

static inline void pixelDibujar(uint16_t x, uint16_t y, uint8_t color) {
    if (x < WIDTH && y < HEIGHT)
        gbt_dibujar_pixel(x, y, color);
}
void lineaHDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color);
void lineaVDibujar(uint16_t x, uint16_t y, uint16_t longitud, uint8_t color);
void rectanguloDibujar(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color);
void rectanguloRelleno(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color);
void bitmapDibujar(const uint8_t* bitmap, uint16_t x, uint16_t y, uint8_t ancho, uint8_t alto, 
                    uint8_t color, uint8_t escala);