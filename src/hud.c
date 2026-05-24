#include "../include/hud.h"
#include "../include/config.h"
#include "../include/piezas.h"
#include <GBT/gbt.h>

void disposicionHudCalcular(tConfigPantalla* config)
{
    if(!config) return;

    uint16_t margen = 10;
    uint16_t alto_caja = 36;  // label escala2 (10px) + offset (3px) + gap (2px) + numero escala2 (14px) + padding

    // === HUD IZQUIERDO ===
    // Centrado dentro de la zona con ~20% de margen horizontal a cada lado
    uint16_t pad_izq = config->ancho_hud_izq / 5;
    if(pad_izq < margen) pad_izq = margen;

    config->hud.next.offset_x_inicial = pad_izq;
    config->hud.next.offset_x_final   = config->ancho_hud_izq - pad_izq;
    config->hud.next.offset_y_inicial = config->offset_tablero_y;
    config->hud.next.offset_y_final   = config->offset_tablero_y + (config->tam_bloque * 4) + 10;

    uint16_t pad_piezas = pad_izq + config->ancho_hud_izq / 8;
    config->hud.piezas.offset_x_inicial = pad_piezas;
    config->hud.piezas.offset_x_final   = config->ancho_hud_izq - pad_piezas;
    config->hud.piezas.offset_y_inicial = config->hud.next.offset_y_final + margen;
    config->hud.piezas.offset_y_final   = config->hud.piezas.offset_y_inicial + (config->tam_bloque * 9) + 10;

    // === HUD DERECHO ===
    uint16_t hud_der_inicio = config->offset_tablero_x + config->ancho_tablero;
    uint16_t pad_der = config->ancho_hud_der / 5;
    if(pad_der < margen) pad_der = margen;
    uint16_t x_ini_der = hud_der_inicio + pad_der;
    uint16_t x_fin_der = config->ancho_ventana - pad_der;

    config->hud.score.offset_x_inicial = x_ini_der;
    config->hud.score.offset_x_final   = x_fin_der;
    config->hud.score.offset_y_inicial = config->offset_tablero_y;
    config->hud.score.offset_y_final   = config->offset_tablero_y + alto_caja;

    // Nivel: caja angosta centrada, numero en escala mayor
    uint16_t ancho_nivel = config->ancho_hud_der / 2;
    uint16_t pad_nivel   = (config->ancho_hud_der - ancho_nivel) / 2;
    config->hud.nivel.offset_x_inicial = hud_der_inicio + pad_nivel;
    config->hud.nivel.offset_x_final   = hud_der_inicio + pad_nivel + ancho_nivel;
    config->hud.nivel.offset_y_inicial = config->hud.score.offset_y_final + margen;
    config->hud.nivel.offset_y_final   = config->hud.nivel.offset_y_inicial + 40;

    config->hud.lineas.offset_x_inicial = x_ini_der;
    config->hud.lineas.offset_x_final   = x_fin_der;
    config->hud.lineas.offset_y_inicial = config->hud.nivel.offset_y_final + margen;
    config->hud.lineas.offset_y_final   = config->hud.lineas.offset_y_inicial + alto_caja;

    config->hud.tiempo.offset_x_inicial = x_ini_der;
    config->hud.tiempo.offset_x_final   = x_fin_der;
    config->hud.tiempo.offset_y_inicial = config->hud.lineas.offset_y_final + margen;
    config->hud.tiempo.offset_y_final   = config->hud.tiempo.offset_y_inicial + alto_caja;

    config->hud.nombre.offset_x_inicial = x_ini_der;
    config->hud.nombre.offset_x_final   = x_fin_der;
    uint16_t nombre_y_ini = config->alto_ventana - alto_caja - margen;
    if (nombre_y_ini < config->hud.tiempo.offset_y_final + margen)
        nombre_y_ini = config->hud.tiempo.offset_y_final + margen;
    config->hud.nombre.offset_y_inicial = nombre_y_ini;
    uint16_t nombre_y_fin = nombre_y_ini + alto_caja;
    config->hud.nombre.offset_y_final = (nombre_y_fin < config->alto_ventana)
                                         ? nombre_y_fin : config->alto_ventana - 1;
}
void hudRecuadrosDibujar(tConfigPantalla* config)
{
    if(!config) return;

    tPosHud cajas[7];
    cajas[0] = config->hud.next;
    cajas[1] = config->hud.piezas;
    cajas[2] = config->hud.score;
    cajas[3] = config->hud.nivel;
    cajas[4] = config->hud.lineas;
    cajas[5] = config->hud.tiempo;
    cajas[6] = config->hud.nombre;

    for (uint8_t k = 0; k < 7; k++) {
        uint16_t x0 = cajas[k].offset_x_inicial;
        uint16_t x1 = cajas[k].offset_x_final;
        uint16_t y0 = cajas[k].offset_y_inicial;
        uint16_t y1 = cajas[k].offset_y_final;

        // Glow exterior 1px en NA (naranja suave)
        for (uint16_t x = x0; x <= x1; x++) {
            if (y0 > 0) gbt_dibujar_pixel(x, y0 - 1, NA);
            gbt_dibujar_pixel(x, y1 + 1, NA);
        }
        for (uint16_t y = y0; y <= y1; y++) {
            if (x0 > 0) gbt_dibujar_pixel(x0 - 1, y, NA);
            gbt_dibujar_pixel(x1 + 1, y, NA);
        }

        // Marco principal en NC (naranja neon)
        for (uint16_t x = x0; x <= x1; x++) {
            gbt_dibujar_pixel(x, y0, NC);
            gbt_dibujar_pixel(x, y1, NC);
        }
        for (uint16_t y = y0; y <= y1; y++) {
            gbt_dibujar_pixel(x0, y, NC);
            gbt_dibujar_pixel(x1, y, NC);
        }
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
        else if (c == ' ')
        {
            pos_x += (ANCHO_LETRA * escala) + 1;
        }
    }
}

// Ancho en píxeles de un texto a una escala dada
static uint16_t hudTextoAncho(const char* texto, uint8_t escala)
{
    if (!texto || !texto[0]) return 0;
    uint16_t n = 0;
    while (texto[n]) n++;
    return (uint16_t)(n * (ANCHO_LETRA * escala + 1)) - 1;
}

// X de inicio para centrar un texto dentro de un recuadro
static uint16_t hudCentrarTexto(tPosHud box, const char* texto, uint8_t escala)
{
    uint16_t bw = box.offset_x_final - box.offset_x_inicial;
    uint16_t lw = hudTextoAncho(texto, escala);
    return box.offset_x_inicial + (bw > lw ? (bw - lw) / 2 : 0);
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

void hudNextDibujar(tTetromino* siguiente, tConfigPantalla* config)
{
    if (!config) return;

    if (!siguiente || siguiente->tipo == TETROMINO_NULO) return;

    const uint8_t* mat = tetrominoObtenerMatrizRotacion(siguiente->tipo, 0);
    if (!mat) return;

    // Bounding box de la pieza dentro de la matriz 4x4
    uint8_t min_f = 4, max_f = 0, min_c = 4, max_c = 0;
    for (uint8_t i = 0; i < 4; i++)
        for (uint8_t j = 0; j < 4; j++)
            if (mat[i * 4 + j]) {
                if (i < min_f) min_f = i;
                if (i > max_f) max_f = i;
                if (j < min_c) min_c = j;
                if (j > max_c) max_c = j;
            }

    uint8_t filas_p = max_f - min_f + 1;
    uint8_t cols_p  = max_c - min_c + 1;
    uint16_t mini   = config->tam_bloque;

    uint16_t box_w      = config->hud.next.offset_x_final - config->hud.next.offset_x_inicial;
    uint16_t area_y_ini = config->hud.next.offset_y_inicial + 2;
    uint16_t area_h     = config->hud.next.offset_y_final - area_y_ini;

    uint16_t x_ini = config->hud.next.offset_x_inicial + (box_w - cols_p * mini) / 2;
    uint16_t y_ini = area_y_ini + (area_h - filas_p * mini) / 2;

    for (uint8_t i = min_f; i <= max_f; i++)
        for (uint8_t j = min_c; j <= max_c; j++)
            if (mat[i * 4 + j]) {
                uint16_t px = x_ini + (j - min_c) * mini;
                uint16_t py = y_ini + (i - min_f) * mini;
                for (uint8_t dy = 0; dy < mini - 1; dy++)
                    for (uint8_t dx = 0; dx < mini - 1; dx++)
                        gbt_dibujar_pixel(px + dx, py + dy, mat[i * 4 + j]);
            }
}

void hudScoreConEtiquetaDibujar(uint32_t* score, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.score.offset_y_inicial + 3;
    hudPalabraDibujar("SCORE", hudCentrarTexto(config->hud.score, "SCORE", 2), y_etiq, CC, 2, config);

    uint32_t score_formateado = *score;
    if (*score > 999999) score_formateado = 999999;

    uint8_t digitos = 6;
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.score.offset_x_final - config->hud.score.offset_x_inicial;
    uint16_t rect_alto = config->hud.score.offset_y_final - config->hud.score.offset_y_inicial - 15;

    uint16_t x_inicio = config->hud.score.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.score.offset_y_inicial + 15 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(score_formateado, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudNivelConEtiquetaDibujar(uint8_t* nivel, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.nivel.offset_y_inicial + 3;
    hudPalabraDibujar("NIVEL", hudCentrarTexto(config->hud.nivel, "NIVEL", 2), y_etiq, CC, 2, config);

    uint32_t nivel_32 = (uint32_t)*nivel;
    uint8_t digitos = contarDigitos(nivel_32);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.nivel.offset_x_final - config->hud.nivel.offset_x_inicial;
    uint16_t rect_alto = config->hud.nivel.offset_y_final - config->hud.nivel.offset_y_inicial - 15;

    uint16_t x_inicio = config->hud.nivel.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.nivel.offset_y_inicial + 15 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(nivel_32, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudLineasConEtiquetaDibujar(uint16_t* lineas, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.lineas.offset_y_inicial + 3;
    hudPalabraDibujar("LINEAS", hudCentrarTexto(config->hud.lineas, "LINEAS", 2), y_etiq, CC, 2, config);

    uint32_t lineas_32 = (uint32_t)*lineas;
    uint8_t digitos = contarDigitos(lineas_32);
    uint8_t espaciado = 2;
    uint8_t ancho_total = ((digitos * ANCHO_NUMERO) + ((digitos - 1) * espaciado)) * escala;

    uint16_t rect_ancho = config->hud.lineas.offset_x_final - config->hud.lineas.offset_x_inicial;
    uint16_t rect_alto = config->hud.lineas.offset_y_final - config->hud.lineas.offset_y_inicial - 15;

    uint16_t x_inicio = config->hud.lineas.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.lineas.offset_y_inicial + 15 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    for (uint8_t i = digitos; i > 0; i--)
    {
        uint8_t digito = extraerDigito(lineas_32, i - 1);
        uint16_t x_pos = x_inicio + (digitos - i) * (ANCHO_NUMERO + espaciado) * escala;
        hudNumeroDibujar(x_pos, y_inicio, digito, CC, escala, config);
    }
}

void hudTiempoConEtiquetaDibujar(uint16_t tiempo_segundos, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.tiempo.offset_y_inicial + 3;
    hudPalabraDibujar("TIEMPO", hudCentrarTexto(config->hud.tiempo, "TIEMPO", 2), y_etiq, CC, 2, config);

    uint8_t minutos = tiempo_segundos / 60;
    uint8_t segundos = tiempo_segundos % 60;

    uint8_t espaciado = 1;
    uint8_t ancho_total = ((4 * ANCHO_NUMERO) + (2 * espaciado) + 2) * escala;

    uint16_t rect_ancho = config->hud.tiempo.offset_x_final - config->hud.tiempo.offset_x_inicial;
    uint16_t rect_alto  = config->hud.tiempo.offset_y_final - config->hud.tiempo.offset_y_inicial - 15;

    uint16_t x_inicio = config->hud.tiempo.offset_x_inicial + (rect_ancho - ancho_total) / 2;
    uint16_t y_inicio = config->hud.tiempo.offset_y_inicial + 15 + (rect_alto - (ALTO_NUMERO * escala)) / 2;

    hudNumeroDibujar(x_inicio, y_inicio, minutos / 10, CC, escala, config);
    x_inicio += (ANCHO_NUMERO + espaciado) * escala;
    hudNumeroDibujar(x_inicio, y_inicio, minutos % 10, CC, escala, config);
    x_inicio += (ANCHO_NUMERO + espaciado) * escala;
    hudDosPuntosDibujar(x_inicio + escala, y_inicio, CC, escala);
    x_inicio += (2 + espaciado) * escala;
    hudNumeroDibujar(x_inicio, y_inicio, segundos / 10, CC, escala, config);
    x_inicio += (ANCHO_NUMERO + espaciado) * escala;
    hudNumeroDibujar(x_inicio, y_inicio, segundos % 10, CC, escala, config);
}

void hudPiezasDibujar(uint16_t conteo[7], tConfigPantalla* config)
{
    if (!config) return;

    uint16_t y_etiq = config->hud.piezas.offset_y_inicial + 3;
    hudPalabraDibujar("PIEZAS", hudCentrarTexto(config->hud.piezas, "PIEZAS", 2), y_etiq, CC, 2, config);

    static const tTipoTetromino tipos[7] = {
        TETROMINO_I, TETROMINO_J, TETROMINO_L, TETROMINO_O,
        TETROMINO_S, TETROMINO_T, TETROMINO_Z
    };

    uint16_t box_x_ini  = config->hud.piezas.offset_x_inicial;
    uint16_t box_x_fin  = config->hud.piezas.offset_x_final;
    uint16_t box_y_ini  = config->hud.piezas.offset_y_inicial;
    uint16_t box_y_fin  = config->hud.piezas.offset_y_final;
    uint16_t box_w      = box_x_fin - box_x_ini;

    uint16_t area_y_ini = box_y_ini + 16;
    uint16_t area_h     = (box_y_fin > area_y_ini) ? (box_y_fin - area_y_ini) : 1;
    uint16_t slot_h     = area_h / 7;

    uint16_t mini = slot_h / 3;
    if (mini < 2) mini = 2;

    // Bloque centrado: 4*mini (pieza) + 4 (gap) + 17 (3 dígitos escala1)
    uint16_t ancho_num  = 3 * (ANCHO_NUMERO + 1) - 1;
    uint16_t content_w  = 4 * mini + 4 + ancho_num;
    uint16_t x_offset   = (box_w > content_w) ? (box_w - content_w) / 2 : 0;
    uint16_t px_base    = box_x_ini + x_offset;
    uint16_t num_x_base = px_base + 4 * mini + 4;

    for (uint8_t t = 0; t < 7; t++)
    {
        const uint8_t* mat = tetrominoObtenerMatrizRotacion(tipos[t], 0);
        if (!mat) continue;

        uint16_t slot_y_ini = area_y_ini + t * slot_h;
        uint16_t slot_cy    = slot_y_ini + slot_h / 2;

        uint8_t min_f = 4, max_f = 0, min_c = 4, max_c = 0;
        for (uint8_t i = 0; i < 4; i++)
            for (uint8_t j = 0; j < 4; j++)
                if (mat[i * 4 + j]) {
                    if (i < min_f) min_f = i;
                    if (i > max_f) max_f = i;
                    if (j < min_c) min_c = j;
                    if (j > max_c) max_c = j;
                }

        uint8_t filas  = max_f - min_f + 1;
        uint8_t cols_p = max_c - min_c + 1;
        uint16_t pieza_h = filas * mini;
        uint16_t pieza_w = cols_p * mini;
        uint16_t py_ini  = (pieza_h < slot_h) ? (slot_cy - pieza_h / 2) : slot_y_ini;
        uint16_t px_ini  = px_base + (4 * mini - pieza_w) / 2;

        for (uint8_t i = min_f; i <= max_f; i++)
            for (uint8_t j = min_c; j <= max_c; j++)
                if (mat[i * 4 + j]) {
                    uint16_t bx = px_ini + (j - min_c) * mini;
                    uint16_t by = py_ini + (i - min_f) * mini;
                    for (uint8_t dy = 0; dy < mini - 1; dy++)
                        for (uint8_t dx = 0; dx < mini - 1; dx++)
                            gbt_dibujar_pixel(bx + dx, by + dy, mat[i * 4 + j]);
                }

        uint32_t cnt     = conteo ? (uint32_t)conteo[t] : 0;
        uint8_t  digitos = contarDigitos(cnt);
        uint16_t num_y   = slot_cy - ALTO_NUMERO / 2;

        for (uint8_t d = digitos; d > 0; d--) {
            uint8_t  dig  = extraerDigito(cnt, d - 1);
            uint16_t xpos = num_x_base + (digitos - d) * (ANCHO_NUMERO + 1);
            hudNumeroDibujar(xpos, num_y, dig, CC, 1, config);
        }
    }
}

void hudTituloNeonDibujar(tConfigPantalla* config)
{
    if (!config) return;

    // Escala según resolución
    uint8_t esc_t = (config->ancho_ventana >= 400) ? 3 : 1;
    uint8_t esc_s = (config->ancho_ventana >= 400) ? 2 : 1;

    uint16_t panel_w = config->ancho_hud_izq;

    // Ancho de "TETRIS" (6 chars) y "GRUPO NEXO" (10 chars con espacio)
    uint16_t titulo_w = (uint16_t)(6  * (ANCHO_LETRA * esc_t + 1) - 1);
    uint16_t sub_w    = (uint16_t)(10 * (ANCHO_LETRA * esc_s + 1) - 1);

    // Posición vertical: debajo de la caja PIEZAS
    uint16_t y_t = config->hud.piezas.offset_y_final + 12;
    uint16_t y_s = y_t + (uint16_t)(ALTO_LETRA * esc_t) + 5;

    // Centrado horizontal dentro del panel izquierdo
    uint16_t x_t = (panel_w > titulo_w) ? (panel_w - titulo_w) / 2 : 0;
    uint16_t x_s = (panel_w > sub_w)    ? (panel_w - sub_w)    / 2 : 0;

    // "TETRIS" en naranja neon brillante
    hudPalabraDibujar("TETRIS",     x_t, y_t, NC, esc_t, config);
    // "GRUPO NEXO" en naranja suave como subtitulo
    hudPalabraDibujar("GRUPO NEXO", x_s, y_s, NA, esc_s, config);
}
