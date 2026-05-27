#include "../include/dibujar.h"
#include "../include/piezas.h"
#include "../include/logica.h"

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

void cuadriculaDibujar(tTablero* tablero, tConfigPantalla* config)
{
    if(!tablero || !config)
    {
        return;
    }
    uint16_t i,j;

    rectanguloRelleno(config->tablero.offset_x, config->tablero.offset_y, config->tablero.ancho_px, config->tablero.alto_px, N, config);

    for(j = 0; j < config->tablero.columnas + 1; j++)
    {
        lineaVDibujar(config->tablero.offset_x + (j * config->tablero.tam_bloque), config->tablero.offset_y, config->tablero.alto_px, CC, config);
    }
    for(i = 0; i < config->tablero.filas_visibles + 1; i++)
    {
        uint8_t color = (i == 0) ? AO : CC;
        lineaHDibujar(config->tablero.offset_x, config->tablero.offset_y + (i * config->tablero.tam_bloque), config->tablero.ancho_px, color, config);
    }
}

void piezasAncladasDibujar(tTablero* t, tConfigPantalla* config)
{
    if(!t || !(t->tablero))
    {
        return;
    }
    for(uint8_t i = FILAS_INVISIBLES; i < config->tablero.filas; i++)
    {
        for(uint8_t j = 0; j < config->tablero.columnas; j++)
        {
            if(t->tablero[i][j] != 0)
            {
                rectanguloRelleno(
                    config->tablero.offset_x + (j * config->tablero.tam_bloque),
                    config->tablero.offset_y + ((i - FILAS_INVISIBLES) * config->tablero.tam_bloque),
                    config->tablero.tam_bloque,
                    config->tablero.tam_bloque,
                    t->tablero[i][j], config);
            }
        }
    }
}

void piezaActualDibujar(tTetromino* tetro, tConfigPantalla* config)
{
    if(!tetro)
    {
        return;
    }
    int x_dibujo;
    int y_dibujo;
    for(uint8_t i = tetro->min_fila; i <= tetro->max_fila; i++)
    {
        for(uint8_t j = tetro->min_col; j <= tetro->max_col; j++)
        {
            if(tetro->matriz[i][j] != 0)
            {
                y_dibujo = tetro->y + i;
                if(y_dibujo < FILAS_INVISIBLES)
                    continue;
                x_dibujo = tetro->x + j;
                if(config->tablero_circular)
                {
                    x_dibujo = ((x_dibujo % config->tablero.columnas) + config->tablero.columnas) % config->tablero.columnas;
                }
                rectanguloRelleno(
                    config->tablero.offset_x + x_dibujo * config->tablero.tam_bloque,
                    config->tablero.offset_y + (y_dibujo - FILAS_INVISIBLES) * config->tablero.tam_bloque,
                    config->tablero.tam_bloque,
                    config->tablero.tam_bloque,
                    tetro->matriz[i][j]
                , config);
            }
        }
    }
}

void piezaGhostDibujar(tTetromino* tetro, int ghost_y, tConfigPantalla* config)
{
    if(!tetro || !config)
    {
        return;
    }
    int x_dibujo;
    int y_dibujo;
    for(uint8_t i = tetro->min_fila; i <= tetro->max_fila; i++)
    {
        for(uint8_t j = tetro->min_col; j <= tetro->max_col; j++)
        {
            if(tetro->matriz[i][j] != 0)
            {
                y_dibujo = ghost_y + i;
                if(y_dibujo < FILAS_INVISIBLES)
                    continue;
                x_dibujo = tetro->x + j;
                if(config->tablero_circular)
                {
                    x_dibujo = ((x_dibujo % config->tablero.columnas) + config->tablero.columnas) % config->tablero.columnas;
                }
                rectanguloRelleno(
                    config->tablero.offset_x + x_dibujo * config->tablero.tam_bloque,
                    config->tablero.offset_y + (y_dibujo - FILAS_INVISIBLES) * config->tablero.tam_bloque,
                    config->tablero.tam_bloque,
                    config->tablero.tam_bloque,
                    GA, config);
            }
        }
    }
}
