#include "../include/hud.h"
#include "../include/config.h"
#include <GBT/gbt.h>

void disposicionHudCalcular(tConfigPantalla* config)
{
    if(!config)
    {
        return;
    }

    uint16_t margen = 10;
    uint16_t alto_recuadro_pequeno = 25;
    //uint16_t ancho_recuadro_hud = config->ancho_hud_izq - (margen * 2);

    // === HUD IZQUIERDO ===
    // Next Piece
    config->hud.next.offset_x_inicial = margen;
    config->hud.next.offset_x_final = config->ancho_hud_izq - margen;
    config->hud.next.offset_y_inicial = config->offset_tablero_y;
    config->hud.next.offset_y_final = config->offset_tablero_y + (config->tam_bloque * 4) + 10;

    // Piezas limpias (debajo de Next)
    config->hud.piezas.offset_x_inicial = margen;
    config->hud.piezas.offset_x_final = config->ancho_hud_izq - margen;
    config->hud.piezas.offset_y_inicial = config->hud.next.offset_y_final+ margen;
    config->hud.piezas.offset_y_final = config->hud.piezas.offset_y_inicial + (config->tam_bloque * 7) + 10;

    // === HUD DERECHO ===
    uint16_t hud_der_inicio = config->offset_tablero_x + config->ancho_tablero;

    // Score (arriba)
    config->hud.score.offset_x_inicial = hud_der_inicio + margen;
    config->hud.score.offset_x_final = config->ancho_ventana - margen;
    config->hud.score.offset_y_inicial = config->offset_tablero_y;
    config->hud.score.offset_y_final = config->offset_tablero_y + alto_recuadro_pequeno;

    // Nivel (debajo de Score)
    config->hud.nivel.offset_x_inicial = hud_der_inicio + margen;
    config->hud.nivel.offset_x_final = config->ancho_ventana - margen;
    config->hud.nivel.offset_y_inicial = config->hud.score.offset_y_final + margen;
    config->hud.nivel.offset_y_final = config->hud.nivel.offset_y_inicial + alto_recuadro_pequeno;

    // Líneas (entre Score y Nivel, o en otra posición)
    config->hud.lineas.offset_x_inicial = hud_der_inicio + margen;
    config->hud.lineas.offset_x_final = config->ancho_ventana - margen;
    config->hud.lineas.offset_y_inicial = config->hud.nivel.offset_y_final + margen;
    config->hud.lineas.offset_y_final = config->hud.lineas.offset_y_inicial + alto_recuadro_pequeno;

    // Tiempo (entre Nivel y Nombre)
    config->hud.tiempo.offset_x_inicial = hud_der_inicio + margen;
    config->hud.tiempo.offset_x_final = config->ancho_ventana - margen;
    config->hud.tiempo.offset_y_inicial = config->hud.nivel.offset_y_final + margen;
    config->hud.tiempo.offset_y_final = config->hud.tiempo.offset_y_inicial + alto_recuadro_pequeno;

    // Nombre (abajo)
    config->hud.nombre.offset_x_inicial = hud_der_inicio + margen;
    config->hud.nombre.offset_x_final = config->ancho_ventana - margen;
    config->hud.nombre.offset_y_inicial = config->alto_ventana - alto_recuadro_pequeno - margen;
    config->hud.nombre.offset_y_final = config->alto_ventana - margen;

}
void hudRecuadrosDibujar(tConfigPantalla* config)
{
    if(!config)
    {
        return;
    }
    uint8_t color = CC;
//                              ===== NEXT PIECE =====
    for(uint16_t x = config->hud.next.offset_x_inicial; x <= config->hud.next.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.next.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.next.offset_y_final, color);
    }
    for(uint16_t y = config->hud.next.offset_y_inicial; y <= config->hud.next.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.next.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.next.offset_x_final, y, color);
    }

//                          ===== PIEZAS LIMPIAS =====
    for(uint16_t x = config->hud.piezas.offset_x_inicial; x <= config->hud.piezas.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.piezas.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.piezas.offset_y_final, color);
    }
    for(uint16_t y = config->hud.piezas.offset_y_inicial; y <= config->hud.piezas.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.piezas.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.piezas.offset_x_final, y, color);
    }

//                              ===== SCORE =====
    for(uint16_t x = config->hud.score.offset_x_inicial; x <= config->hud.score.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.score.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.score.offset_y_final, color);
    }
    for(uint16_t y = config->hud.score.offset_y_inicial; y <= config->hud.score.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.score.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.score.offset_x_final, y, color);
    }

//                              ===== NIVEL =====
    for(uint16_t x = config->hud.nivel.offset_x_inicial; x <= config->hud.nivel.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.nivel.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.nivel.offset_y_final, color);
    }
    for(uint16_t y = config->hud.nivel.offset_y_inicial; y <= config->hud.nivel.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.nivel.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.nivel.offset_x_final, y, color);
    }

//                              ===== LINEAS =====
    for(uint16_t x = config->hud.lineas.offset_x_inicial; x <= config->hud.lineas.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.lineas.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.lineas.offset_y_final, color);
    }
    for(uint16_t y = config->hud.lineas.offset_y_inicial; y <= config->hud.lineas.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.lineas.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.lineas.offset_x_final, y, color);
    }

//                              ===== TIEMPO =====
    for(uint16_t x = config->hud.tiempo.offset_x_inicial; x <= config->hud.tiempo.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.tiempo.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.tiempo.offset_y_final, color);
    }
    for(uint16_t y = config->hud.tiempo.offset_y_inicial; y <= config->hud.tiempo.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.tiempo.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.tiempo.offset_x_final, y, color);
    }

//                              ===== NOMBRE =====
    for(uint16_t x = config->hud.nombre.offset_x_inicial; x <= config->hud.nombre.offset_x_final; x++)
    {
        gbt_dibujar_pixel(x, config->hud.nombre.offset_y_inicial, color);
        gbt_dibujar_pixel(x, config->hud.nombre.offset_y_final, color);
    }
    for(uint16_t y = config->hud.nombre.offset_y_inicial; y <= config->hud.nombre.offset_y_final; y++)
    {
        gbt_dibujar_pixel(config->hud.nombre.offset_x_inicial, y, color);
        gbt_dibujar_pixel(config->hud.nombre.offset_x_final, y, color);
    }
}

void hudLetraDibujar(const uint8_t (*letra)[ANCHO_LETRA], uint16_t x, uint16_t y, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (!config || !letra) return;

    for (uint8_t i = 0; i < ALTO_LETRA; i++)
    {
        for (uint8_t j = 0; j < ANCHO_LETRA; j++)
        {
            if (letra[i][j] != 0)
            {
                for (uint8_t sy = 0; sy < escala; sy++)
                    for (uint8_t sx = 0; sx < escala; sx++)
                        gbt_dibujar_pixel(x + j * escala + sx, y + i * escala + sy, color);
            }
        }
    }
}

void hudPalabraDibujar(const char* palabra, uint16_t x, uint16_t y, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (!config || !palabra) return;

    const uint8_t (*letras[26])[ANCHO_LETRA] = {
        letra_A, letra_B, letra_C, letra_D, letra_E, letra_F, letra_G, letra_H, letra_I, letra_J,
        letra_K, letra_L, letra_M, letra_N, letra_O, letra_P, letra_Q, letra_R, letra_S, letra_T,
        letra_U, letra_V, letra_W, letra_X, letra_Y, letra_Z
    };

    uint16_t pos_x = x;
    for (int i = 0; palabra[i] != '\0'; i++)
    {
        char c = palabra[i];
        if (c >= 'A' && c <= 'Z')
        {
            int idx = c - 'A';
            hudLetraDibujar(letras[idx], pos_x, y, color, escala, config);
            pos_x += (ANCHO_LETRA * escala) + 1;
        }
        else if (c >= 'a' && c <= 'z')
        {
            int idx = c - 'a';
            hudLetraDibujar(letras[idx], pos_x, y, color, escala, config);
            pos_x += (ANCHO_LETRA * escala) + 1;
        }
    }
}

// Función auxiliar para contar dígitos
static uint8_t contarDigitos(uint32_t numero)
{
    if (numero == 0) return 1;
    uint8_t count = 0;
    while (numero > 0)
    {
        count++;
        numero /= 10;
    }
    return count;
}

// Función auxiliar para extraer dígito en posición
static uint8_t extraerDigito(uint32_t numero, uint8_t posicion)
{
    for (uint8_t i = 0; i < posicion; i++)
    {
        numero /= 10;
    }
    return numero % 10;
}

void hudNumeroDibujar(uint16_t x, uint16_t y, uint8_t numero, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (!config || numero > 9)
    {
        return;
    }

    static const uint8_t (*numeros[10])[ANCHO_NUMERO] =
    {
        cero, uno, dos, tres, cuatro,
        cinco, seis, siete, ocho, nueve
    };

    const uint8_t (*matriz)[ANCHO_NUMERO] = numeros[numero];

    for (uint8_t i = 0; i < ALTO_NUMERO; i++)
    {
        for (uint8_t j = 0; j < ANCHO_NUMERO; j++)
        {
            if (matriz[i][j] != 0)
            {
                for (uint8_t sy = 0; sy < escala; sy++)
                    for (uint8_t sx = 0; sx < escala; sx++)
                        gbt_dibujar_pixel(x + j * escala + sx, y + i * escala + sy, color);
            }
        }
    }
}
static void hudDosPuntosDibujar(uint16_t x, uint16_t y, uint8_t color, uint8_t escala)
{
    uint16_t y_centro = y + (ALTO_NUMERO * escala / 2);
    for (uint8_t sx = 0; sx < escala; sx++)
        gbt_dibujar_pixel(x + sx, y_centro - 2 * escala, color);
    for (uint8_t sx = 0; sx < escala; sx++)
        gbt_dibujar_pixel(x + sx, y_centro + 2 * escala, color);
}

void hudTiempoDibujar(uint16_t tiempo_segundos, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint8_t minutos = tiempo_segundos / 60;
    uint8_t segundos = tiempo_segundos % 60;

    uint8_t espaciado = 1;
    uint8_t ancho_total = ((4 * ANCHO_NUMERO) + (2 * espaciado) + 2) * escala;

    uint16_t rect_ancho = config->hud.tiempo.offset_x_final - config->hud.tiempo.offset_x_inicial;
    uint16_t rect_alto = config->hud.tiempo.offset_y_final - config->hud.tiempo.offset_y_inicial;

    uint16_t x_inicio = config->hud.tiempo.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.tiempo.offset_y_inicial + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    // Dígito minuto decena
    hudNumeroDibujar(x_inicio, y_inicio, minutos / 10, CC, escala, config);
    x_inicio += (ANCHO_NUMERO + espaciado) * escala;

    // Dígito minuto unidad
    hudNumeroDibujar(x_inicio, y_inicio, minutos % 10, CC, escala, config);
    x_inicio += (ANCHO_NUMERO + espaciado) * escala;

    // Dos puntos
    hudDosPuntosDibujar(x_inicio + escala, y_inicio, CC, escala);
    x_inicio += (2 + espaciado) * escala;

    // Dígito segundo decena
    hudNumeroDibujar(x_inicio, y_inicio, segundos / 10, CC, escala, config);
    x_inicio += (ANCHO_NUMERO + espaciado) * escala;

    // Dígito segundo unidad
    hudNumeroDibujar(x_inicio, y_inicio, segundos % 10, CC, escala, config);
}

void hudScoreDibujar(uint32_t score, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint8_t digitos = contarDigitos(score);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.score.offset_x_final - config->hud.score.offset_x_inicial;
    uint16_t rect_alto = config->hud.score.offset_y_final - config->hud.score.offset_y_inicial;

    uint16_t x_inicio = config->hud.score.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.score.offset_y_inicial + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(score, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudNivelDibujar(uint8_t nivel, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint32_t nivel_32 = (uint32_t)nivel;
    uint8_t digitos = contarDigitos(nivel_32);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.nivel.offset_x_final - config->hud.nivel.offset_x_inicial;
    uint16_t rect_alto = config->hud.nivel.offset_y_final - config->hud.nivel.offset_y_inicial;

    uint16_t x_inicio = config->hud.nivel.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.nivel.offset_y_inicial + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(nivel_32, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudLineasDibujar(uint16_t lineas, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint32_t lineas_32 = (uint32_t)lineas;
    uint8_t digitos = contarDigitos(lineas_32);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.piezas.offset_x_final - config->hud.piezas.offset_x_inicial;
    uint16_t rect_alto = config->hud.piezas.offset_y_final - config->hud.piezas.offset_y_inicial;

    uint16_t x_inicio = config->hud.piezas.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.piezas.offset_y_inicial + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(lineas_32, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudNextDibujar(tConfigPantalla* config)
{
    if (!config) return;

    uint16_t rect_ancho = config->hud.next.offset_x_final - config->hud.next.offset_x_inicial;
    uint16_t rect_alto = config->hud.next.offset_y_final - config->hud.next.offset_y_inicial;

    uint16_t y_etiq = config->hud.next.offset_y_inicial + 3;
    hudPalabraDibujar("NEXT", config->hud.next.offset_x_inicial + 5, y_etiq, CC, 1, config);
}

void hudScoreConEtiquetaDibujar(uint32_t score, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.score.offset_y_inicial + 1;
    hudPalabraDibujar("SCORE", config->hud.score.offset_x_inicial + 5, y_etiq, CC, 1, config);

    uint32_t score_formateado = score;
    if (score > 999999) score_formateado = 999999;

    uint8_t digitos = 6;
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.score.offset_x_final - config->hud.score.offset_x_inicial;
    uint16_t rect_alto = config->hud.score.offset_y_final - config->hud.score.offset_y_inicial - 10;

    uint16_t x_inicio = config->hud.score.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.score.offset_y_inicial + 10 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(score_formateado, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudNivelConEtiquetaDibujar(uint8_t nivel, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.nivel.offset_y_inicial + 1;
    hudPalabraDibujar("NIVEL", config->hud.nivel.offset_x_inicial + 5, y_etiq, CC, 1, config);

    uint32_t nivel_32 = (uint32_t)nivel;
    uint8_t digitos = contarDigitos(nivel_32);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.nivel.offset_x_final - config->hud.nivel.offset_x_inicial;
    uint16_t rect_alto = config->hud.nivel.offset_y_final - config->hud.nivel.offset_y_inicial - 10;

    uint16_t x_inicio = config->hud.nivel.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.nivel.offset_y_inicial + 10 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(nivel_32, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudLineasConEtiquetaDibujar(uint16_t lineas, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.lineas.offset_y_inicial + 1;
    hudPalabraDibujar("LINEAS", config->hud.lineas.offset_x_inicial + 5, y_etiq, CC, 1, config);

    uint32_t lineas_32 = (uint32_t)lineas;
    uint8_t digitos = contarDigitos(lineas_32);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.lineas.offset_x_final - config->hud.lineas.offset_x_inicial;
    uint16_t rect_alto = config->hud.lineas.offset_y_final - config->hud.lineas.offset_y_inicial - 10;

    uint16_t x_inicio = config->hud.lineas.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.lineas.offset_y_inicial + 10 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(lineas_32, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}
