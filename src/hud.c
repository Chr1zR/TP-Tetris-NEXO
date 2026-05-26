#include "../include/hud.h"
#include "../include/config.h"
#include "../include/piezas.h"
#include "../include/dibujar.h"
#include "../include/juego.h"
#include <string.h>
#include <stdio.h>
#include <GBT/gbt.h>

#define MARGEN 10
#define ALTO_CAJA 36
#define ETIQUETA_DY 3
#define ETIQUETA_ESCALA 2
#define ETIQUETA_ALTO 15

static void hudDigitoDibujar(uint16_t x, uint16_t y, uint8_t numero, uint8_t color, uint8_t escala, tConfigPantalla* config);

static void posFijar(tPosHud* pos, uint16_t x_inicial, uint16_t y_inicial, uint16_t x_final, uint16_t y_final)
{
    pos->offset_x_inicial = x_inicial;
    pos->offset_y_inicial = y_inicial;
    pos->offset_x_final = x_final;
    pos->offset_y_final = y_final;
}

static void tetrominoLimitesObtener(const uint8_t* mat, uint8_t* fila_min, uint8_t* fila_max, uint8_t* col_min, uint8_t* col_max)
{
    *fila_min = 4; *fila_max = 0; *col_min = 4; *col_max = 0;
    for (uint8_t i = 0; i < 4; i++)
        for (uint8_t j = 0; j < 4; j++)
            if (mat[i * 4 + j]) {
                if (i < *fila_min) *fila_min = i;
                if (i > *fila_max) *fila_max = i;
                if (j < *col_min) *col_min = j;
                if (j > *col_max) *col_max = j;
            }
}

void disposicionHudCalcular(tConfigPantalla* config)
{
    if (!config) return;

    uint16_t pad_izq = config->hud.ancho_izq / 5;
    if (pad_izq < MARGEN) pad_izq = MARGEN;
    uint16_t tam_bloque = config->tablero.tam_bloque;

    posFijar(&config->hud.disposicion.next,
        pad_izq, config->tablero.offset_y,
        config->hud.ancho_izq - pad_izq, config->tablero.offset_y + tam_bloque * 4 + 10);

    uint16_t pad_piezas = pad_izq + config->hud.ancho_izq / 8;
    posFijar(&config->hud.disposicion.piezas,
        pad_piezas, config->hud.disposicion.next.offset_y_final + MARGEN,
        config->hud.ancho_izq - pad_piezas, config->hud.disposicion.next.offset_y_final + MARGEN + tam_bloque * 9 + 10);

    uint16_t der_inicio = config->tablero.offset_x + config->tablero.ancho_px;
    uint16_t pad_der = config->hud.ancho_der / 5;
    if (pad_der < MARGEN) pad_der = MARGEN;
    uint16_t x_ini_der = der_inicio + pad_der, x_fin_der = config->ventana.ancho - pad_der;
    uint16_t y = config->tablero.offset_y;

    posFijar(&config->hud.disposicion.score, x_ini_der, y, x_fin_der, y + ALTO_CAJA);
    y += ALTO_CAJA + MARGEN;

    uint16_t ancho_nivel = config->hud.ancho_der / 2, pad_nivel = (config->hud.ancho_der - ancho_nivel) / 2;
    posFijar(&config->hud.disposicion.nivel, der_inicio + pad_nivel, y, der_inicio + pad_nivel + ancho_nivel, y + 40);
    y += 40 + MARGEN;

    posFijar(&config->hud.disposicion.lineas, x_ini_der, y, x_fin_der, y + ALTO_CAJA);
    y += ALTO_CAJA + MARGEN;

    posFijar(&config->hud.disposicion.tiempo, x_ini_der, y, x_fin_der, y + ALTO_CAJA);
    y += ALTO_CAJA + MARGEN;

    uint16_t nombre_y = config->ventana.alto - ALTO_CAJA - MARGEN;
    if (nombre_y < y) nombre_y = y;
    uint16_t nombre_y_fin = nombre_y + ALTO_CAJA;
    posFijar(&config->hud.disposicion.nombre, x_ini_der, nombre_y, x_fin_der,
        nombre_y_fin < config->ventana.alto ? nombre_y_fin : config->ventana.alto - 1);
}

void hudRecuadrosDibujar(tConfigPantalla* config)
{
    if (!config) return;
    tPosHud cajas[7] = {
        config->hud.disposicion.next, config->hud.disposicion.piezas,
        config->hud.disposicion.score, config->hud.disposicion.nivel,
        config->hud.disposicion.lineas, config->hud.disposicion.tiempo,
        config->hud.disposicion.nombre
    };
    for (uint8_t i = 0; i < 7; i++)
        rectanguloDibujar(cajas[i].offset_x_inicial, cajas[i].offset_y_inicial,
            cajas[i].offset_x_final - cajas[i].offset_x_inicial,
            cajas[i].offset_y_final - cajas[i].offset_y_inicial, NA, config);
}

uint8_t hudCaracterAncho(char c)
{
    if (c >= 'A' && c <= 'Z') return ancho_letras[c - 'A'];
    if (c >= 'a' && c <= 'z') return ancho_letras[c - 'a'];
    if (c >= '0' && c <= '9') return ancho_digitos[c - '0'];
    if (c == ' ') return 4;
    return 2;
}

static void hudLetraDibujar(uint16_t x, uint16_t y, char c, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (c >= 'A' && c <= 'Z') {
        uint8_t indice = c - 'A';
        uint8_t ancho = ancho_letras[indice];
        switch (indice) {
            case 0: bitmapDibujar(&letra_A[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 1: bitmapDibujar(&letra_B[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 2: bitmapDibujar(&letra_C[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 3: bitmapDibujar(&letra_D[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 4: bitmapDibujar(&letra_E[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 5: bitmapDibujar(&letra_F[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 6: bitmapDibujar(&letra_G[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 7: bitmapDibujar(&letra_H[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 8: bitmapDibujar(&letra_I[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 9: bitmapDibujar(&letra_J[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 10: bitmapDibujar(&letra_K[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 11: bitmapDibujar(&letra_L[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 12: bitmapDibujar(&letra_M[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 13: bitmapDibujar(&letra_N[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 14: bitmapDibujar(&letra_O[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 15: bitmapDibujar(&letra_P[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 16: bitmapDibujar(&letra_Q[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 17: bitmapDibujar(&letra_R[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 18: bitmapDibujar(&letra_S[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 19: bitmapDibujar(&letra_T[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 20: bitmapDibujar(&letra_U[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 21: bitmapDibujar(&letra_V[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 22: bitmapDibujar(&letra_W[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 23: bitmapDibujar(&letra_X[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 24: bitmapDibujar(&letra_Y[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 25: bitmapDibujar(&letra_Z[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
        }
    } else if (c >= 'a' && c <= 'z') {
        uint8_t indice = c - 'a';
        uint8_t ancho = ancho_letras[indice];
        switch (indice) {
            case 0: bitmapDibujar(&letra_A[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 1: bitmapDibujar(&letra_B[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 2: bitmapDibujar(&letra_C[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 3: bitmapDibujar(&letra_D[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 4: bitmapDibujar(&letra_E[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 5: bitmapDibujar(&letra_F[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 6: bitmapDibujar(&letra_G[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 7: bitmapDibujar(&letra_H[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 8: bitmapDibujar(&letra_I[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 9: bitmapDibujar(&letra_J[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 10: bitmapDibujar(&letra_K[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 11: bitmapDibujar(&letra_L[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 12: bitmapDibujar(&letra_M[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 13: bitmapDibujar(&letra_N[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 14: bitmapDibujar(&letra_O[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 15: bitmapDibujar(&letra_P[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 16: bitmapDibujar(&letra_Q[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 17: bitmapDibujar(&letra_R[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 18: bitmapDibujar(&letra_S[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 19: bitmapDibujar(&letra_T[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 20: bitmapDibujar(&letra_U[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 21: bitmapDibujar(&letra_V[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 22: bitmapDibujar(&letra_W[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 23: bitmapDibujar(&letra_X[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 24: bitmapDibujar(&letra_Y[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
            case 25: bitmapDibujar(&letra_Z[0][0], x, y, ancho, ALTO_LETRA, color, escala, config); break;
        }
    }
}

void hudPalabraDibujar(const char* palabra, uint16_t pos_x, uint16_t pos_y, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (!config || !palabra) return;
    for (int i = 0; palabra[i]; i++) {
        char c = palabra[i];
        if (c == ' ') {
            pos_x += 4 * escala + 1;
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            uint8_t ancho_real = hudCaracterAncho(c);
            hudLetraDibujar(pos_x, pos_y, c, color, escala, config);
            pos_x += ancho_real * escala + 1;
        } else if (c >= '0' && c <= '9') {
            uint8_t digito = c - '0';
            uint8_t ancho_real = ancho_digitos[digito];
            hudDigitoDibujar(pos_x, pos_y, digito, color, escala, config);
            pos_x += ancho_real * escala + 1;
        } else if (c == '|') {
            for (uint8_t dy = 0; dy < ALTO_LETRA * escala; dy++)
                for (uint8_t dx = 0; dx < 2 * escala; dx++)
                    pixelDibujar(pos_x + dx, pos_y + dy, color, config->ventana.ancho, config->ventana.alto);
            pos_x += 2 * escala + 1;
        } else if (c == '.') {
            uint16_t y_punto = pos_y + (ALTO_NUMERO - 2) * escala;
            for (uint8_t dy = 0; dy < 2 * escala; dy++)
                for (uint8_t dx = 0; dx < 2 * escala; dx++)
                    pixelDibujar(pos_x + dx, y_punto + dy, color, config->ventana.ancho, config->ventana.alto);
            pos_x += 2 * escala + 1;
        } else {
            pos_x += 2 * escala + 1;
        }
    }
}

static uint16_t hudTextoAncho(const char* texto, uint8_t escala)
{
    if (!texto || !texto[0]) return 0;
    uint16_t ancho = 0;
    for (int i = 0; texto[i]; i++) {
        char c = texto[i];
        if (c == ' ') {
            ancho += 4 * escala + 1;
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            ancho += hudCaracterAncho(c) * escala + 1;
        } else if (c >= '0' && c <= '9') {
            ancho += ancho_digitos[c - '0'] * escala + 1;
        } else if (c == '|' || c == '.') {
            ancho += 2 * escala + 1;
        } else {
            ancho += 2 * escala + 1;
        }
    }
    return ancho > 0 ? ancho - 1 : 0;
}

static uint16_t hudCentrarTexto(tPosHud recuadro, const char* texto, uint8_t escala)
{
    uint16_t ancho = recuadro.offset_x_final - recuadro.offset_x_inicial;
    uint16_t largo = hudTextoAncho(texto, escala);
    return recuadro.offset_x_inicial + (ancho > largo ? (ancho - largo) / 2 : 0);
}

static uint8_t contarDigitos(uint32_t numero)
{
    if (numero == 0) return 1;
    uint8_t cant = 0;
    while (numero > 0) { cant++; numero /= 10; }
    return cant;
}

static uint8_t extraerDigito(uint32_t numero, uint8_t posicion)
{
    for (uint8_t i = 0; i < posicion; i++) numero /= 10;
    return numero % 10;
}

static void hudDigitoDibujar(uint16_t x, uint16_t y, uint8_t numero, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (numero > 9) return;
    uint8_t ancho = ancho_digitos[numero];
    switch (numero) {
        case 0: bitmapDibujar(&cero[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 1: bitmapDibujar(&uno[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 2: bitmapDibujar(&dos[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 3: bitmapDibujar(&tres[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 4: bitmapDibujar(&cuatro[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 5: bitmapDibujar(&cinco[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 6: bitmapDibujar(&seis[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 7: bitmapDibujar(&siete[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 8: bitmapDibujar(&ocho[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
        case 9: bitmapDibujar(&nueve[0][0], x, y, ancho, ALTO_NUMERO, color, escala, config); break;
    }
}

static void hudDosPuntosDibujar(uint16_t x, uint16_t y, uint8_t color, uint8_t escala)
{
    uint16_t y_centro = y + (ALTO_NUMERO * escala / 2);
    for (uint8_t i = 0; i < escala; i++) {
        gbt_dibujar_pixel(x + i, y_centro - 2 * escala, color);
        gbt_dibujar_pixel(x + i, y_centro + 2 * escala, color);
    }
}

static void hudValorEnCaja(tPosHud caja, const char* etiqueta, uint32_t valor, uint8_t escala, tConfigPantalla* config)
{
    hudPalabraDibujar(etiqueta, hudCentrarTexto(caja, etiqueta, ETIQUETA_ESCALA),
        caja.offset_y_inicial + ETIQUETA_DY, CC, ETIQUETA_ESCALA, config);
    uint8_t digitos = contarDigitos(valor);
    uint8_t espaciado = 2;
    uint16_t ancho_valor = 0;
    for (uint8_t i = 0; i < digitos; i++) {
        uint8_t d = extraerDigito(valor, digitos - 1 - i);
        ancho_valor += (ancho_digitos[d] + espaciado) * escala;
    }
    ancho_valor -= espaciado * escala;
    uint16_t ancho_caja = caja.offset_x_final - caja.offset_x_inicial;
    uint16_t alto_caja = caja.offset_y_final - caja.offset_y_inicial - ETIQUETA_ALTO;
    uint16_t x = caja.offset_x_inicial + (ancho_caja - ancho_valor) / 2;
    uint16_t y = caja.offset_y_inicial + ETIQUETA_ALTO + (alto_caja - ALTO_NUMERO * escala) / 2;
    for (uint8_t i = 0; i < digitos; i++) {
        uint8_t d = extraerDigito(valor, digitos - 1 - i);
        uint8_t ancho_d = ancho_digitos[d];
        hudDigitoDibujar(x, y, d, CC, escala, config);
        x += (ancho_d + espaciado) * escala;
    }
}

void hudScoreDibujar(uint32_t score, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;
    hudValorEnCaja(config->hud.disposicion.score, "SCORE", score > 999999 ? 999999 : score, escala, config);
}

void hudNivelDibujar(uint8_t nivel, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;
    hudValorEnCaja(config->hud.disposicion.nivel, "NIVEL", nivel, escala, config);
}

void hudLineasDibujar(uint16_t lineas, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;
    hudValorEnCaja(config->hud.disposicion.lineas, "LINEAS", lineas, escala, config);
}

void hudTiempoDibujar(uint16_t segundos, uint8_t escala, tConfigPantalla* config)
{
    if (!config) return;
    tPosHud caja = config->hud.disposicion.tiempo;
    hudPalabraDibujar("TIEMPO", hudCentrarTexto(caja, "TIEMPO", ETIQUETA_ESCALA),
        caja.offset_y_inicial + ETIQUETA_DY, CC, ETIQUETA_ESCALA, config);
    uint8_t minutos = segundos / 60, seg_restantes = segundos % 60, espaciado = 1;
    uint16_t ancho_total = 0;
    uint8_t m1 = minutos / 10, m2 = minutos % 10;
    uint8_t s1 = seg_restantes / 10, s2 = seg_restantes % 10;
    ancho_total += (ancho_digitos[m1] + espaciado) * escala;
    ancho_total += (ancho_digitos[m2] + espaciado) * escala;
    ancho_total += (2 + espaciado) * escala;
    ancho_total += (ancho_digitos[s1] + espaciado) * escala;
    ancho_total += ancho_digitos[s2] * escala;
    uint16_t ancho_caja = caja.offset_x_final - caja.offset_x_inicial;
    uint16_t alto_caja = caja.offset_y_final - caja.offset_y_inicial - ETIQUETA_ALTO;
    uint16_t x = caja.offset_x_inicial + (ancho_caja - ancho_total) / 2;
    uint16_t y = caja.offset_y_inicial + ETIQUETA_ALTO + (alto_caja - ALTO_NUMERO * escala) / 2;
    hudDigitoDibujar(x, y, m1, CC, escala, config); x += (ancho_digitos[m1] + espaciado) * escala;
    hudDigitoDibujar(x, y, m2, CC, escala, config); x += (ancho_digitos[m2] + espaciado) * escala;
    hudDosPuntosDibujar(x + escala, y, CC, escala); x += (2 + espaciado) * escala;
    hudDigitoDibujar(x, y, s1, CC, escala, config); x += (ancho_digitos[s1] + espaciado) * escala;
    hudDigitoDibujar(x, y, s2, CC, escala, config);
}

void hudNextDibujar(tTetromino* siguiente, tConfigPantalla* config)
{
    if (!config || !siguiente || siguiente->tipo == TETROMINO_NULO) return;
    const uint8_t* mat = tetrominoObtenerMatrizRotacion(siguiente->tipo, 0);
    if (!mat) return;
    uint8_t fila_min, fila_max, col_min, col_max;
    tetrominoLimitesObtener(mat, &fila_min, &fila_max, &col_min, &col_max);
    uint16_t tam_mini = config->tablero.tam_bloque;
    uint16_t ancho_caja = config->hud.disposicion.next.offset_x_final - config->hud.disposicion.next.offset_x_inicial;
    uint16_t area_y = config->hud.disposicion.next.offset_y_inicial + 2;
    uint16_t area_alto = config->hud.disposicion.next.offset_y_final - area_y;
    uint16_t x_ini = config->hud.disposicion.next.offset_x_inicial + (ancho_caja - (col_max - col_min + 1) * tam_mini) / 2;
    uint16_t y_ini = area_y + (area_alto - (fila_max - fila_min + 1) * tam_mini) / 2;
    for (uint8_t i = fila_min; i <= fila_max; i++)
        for (uint8_t j = col_min; j <= col_max; j++)
            if (mat[i * 4 + j]) {
                uint16_t px = x_ini + (j - col_min) * tam_mini, py = y_ini + (i - fila_min) * tam_mini;
                for (uint8_t dy = 0; dy < tam_mini - 1; dy++)
                    for (uint8_t dx = 0; dx < tam_mini - 1; dx++)
                        gbt_dibujar_pixel(px + dx, py + dy, mat[i * 4 + j]);
            }
}

void hudPiezasDibujar(uint16_t conteo[7], tConfigPantalla* config)
{
    if (!config) return;
    tPosHud caja = config->hud.disposicion.piezas;
    hudPalabraDibujar("PIEZAS", hudCentrarTexto(caja, "PIEZAS", ETIQUETA_ESCALA),
        caja.offset_y_inicial + ETIQUETA_DY, CC, ETIQUETA_ESCALA, config);
    static const tTipoTetromino tipos[7] = {
        TETROMINO_I, TETROMINO_J, TETROMINO_L, TETROMINO_O,
        TETROMINO_S, TETROMINO_T, TETROMINO_Z
    };
    uint16_t ancho_caja = caja.offset_x_final - caja.offset_x_inicial;
    uint16_t area_y = caja.offset_y_inicial + 16;
    uint16_t area_alto = (caja.offset_y_final > area_y) ? caja.offset_y_final - area_y : 1;
    uint16_t alto_slot = area_alto / 7, tam_mini = alto_slot / 3;
    if (tam_mini < 2) tam_mini = 2;
    uint16_t ancho_num = 3 * (3 + 1) - 1;
    uint16_t ancho_contenido = 4 * tam_mini + 4 + ancho_num;
    uint16_t x_base = caja.offset_x_inicial + ((ancho_caja > ancho_contenido) ? (ancho_caja - ancho_contenido) / 2 : 0);
    uint16_t x_num_base = x_base + 4 * tam_mini + 4;
    for (uint8_t t = 0; t < 7; t++) {
        const uint8_t* mat = tetrominoObtenerMatrizRotacion(tipos[t], 0);
        if (!mat) continue;
        uint16_t slot_y = area_y + t * alto_slot, slot_centro_y = slot_y + alto_slot / 2;
        uint8_t fila_min, fila_max, col_min, col_max;
        tetrominoLimitesObtener(mat, &fila_min, &fila_max, &col_min, &col_max);
        uint8_t filas_pieza = fila_max - fila_min + 1, cols_pieza = col_max - col_min + 1;
        uint16_t alto_pieza = filas_pieza * tam_mini, ancho_pieza = cols_pieza * tam_mini;
        uint16_t y_pieza = (alto_pieza < alto_slot) ? slot_centro_y - alto_pieza / 2 : slot_y;
        uint16_t x_pieza = x_base + (4 * tam_mini - ancho_pieza) / 2;
        for (uint8_t i = fila_min; i <= fila_max; i++)
            for (uint8_t j = col_min; j <= col_max; j++)
                if (mat[i * 4 + j]) {
                    uint16_t bx = x_pieza + (j - col_min) * tam_mini, by = y_pieza + (i - fila_min) * tam_mini;
                    for (uint8_t dy = 0; dy < tam_mini - 1; dy++)
                        for (uint8_t dx = 0; dx < tam_mini - 1; dx++)
                            gbt_dibujar_pixel(bx + dx, by + dy, mat[i * 4 + j]);
                }
        uint32_t conteo_pieza = conteo ? conteo[t] : 0;
        uint8_t digitos = contarDigitos(conteo_pieza);
        uint16_t y_num = slot_centro_y - ALTO_NUMERO / 2;
        for (uint8_t i = 0; i < digitos; i++) {
            uint8_t d = extraerDigito(conteo_pieza, digitos - 1 - i);
            hudDigitoDibujar(x_num_base + i * 4, y_num, d, CC, 1, config);
        }
    }
}

void hudTituloNeonDibujar(tConfigPantalla* config)
{
    if (!config) return;
    uint8_t escala_titulo = (config->ventana.ancho >= 400) ? 3 : 1;
    uint8_t escala_sub = (config->ventana.ancho >= 400) ? 2 : 1;
    uint16_t ancho_panel = config->hud.ancho_izq;
    uint16_t ancho_titulo = hudTextoAncho("TETRIS", escala_titulo);
    uint16_t ancho_sub = hudTextoAncho("GRUPO NEXO", escala_sub);
    uint16_t y_titulo = config->hud.disposicion.piezas.offset_y_final + 12;
    uint16_t y_sub = y_titulo + ALTO_LETRA * escala_titulo + 5;
    hudPalabraDibujar("TETRIS", (ancho_panel > ancho_titulo ? (ancho_panel - ancho_titulo) / 2 : 0), y_titulo, NC, escala_titulo, config);
    hudPalabraDibujar("GRUPO NEXO", (ancho_panel > ancho_sub ? (ancho_panel - ancho_sub) / 2 : 0), y_sub, NA, escala_sub, config);
}

static void panelDibujar(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color, tConfigPantalla* config)
{
    rectanguloRelleno(x, y, ancho, alto, N, config);
    rectanguloDibujar(x, y, ancho, alto, color, config);
}

static void textoCentradoDibujar(const char* texto, uint16_t centro_x, uint16_t centro_y, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (!config || !texto) return;
    hudPalabraDibujar(texto, centro_x - hudTextoAncho(texto, escala) / 2, centro_y - (ALTO_LETRA * escala) / 2, color, escala, config);
}

static void menuOpcionesDibujar(const char** opciones, uint8_t cantidad, uint16_t centro_x, uint16_t* y, uint8_t seleccion, tConfigPantalla* config)
{
    for (uint8_t i = 0; i < cantidad; i++) {
        if (i == seleccion) {
            uint16_t ancho_opcion = hudTextoAncho(opciones[i], ETIQUETA_ESCALA);
            uint16_t x_opcion = centro_x - ancho_opcion / 2 - 6, y_opcion = *y - 8;
            rectanguloRelleno(x_opcion, y_opcion, ancho_opcion + 12, ALTO_LETRA * ETIQUETA_ESCALA + 12, AO, config);
            rectanguloDibujar(x_opcion, y_opcion, ancho_opcion + 12, ALTO_LETRA * ETIQUETA_ESCALA + 12, CC, config);
            textoCentradoDibujar(opciones[i], centro_x, *y, CC, ETIQUETA_ESCALA, config);
        } else {
            textoCentradoDibujar(opciones[i], centro_x, *y, NA, ETIQUETA_ESCALA, config);
        }
        *y += ALTO_LETRA * ETIQUETA_ESCALA + 14;
    }
}

void cubrirPausaDibujar(tConfigPantalla* config, uint8_t seleccion)
{
    if (!config) return;
    uint16_t ancho_panel = 280, alto_panel = 180;
    uint16_t x_panel = (config->ventana.ancho - ancho_panel) / 2, y_panel = (config->ventana.alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, C, config);
    uint16_t centro_x = x_panel + ancho_panel / 2, y = y_panel + 25;
    textoCentradoDibujar("PAUSA", centro_x, y, CC, 3, config);
    y += ALTO_LETRA * 3 + 10;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 15;
    static const char* opciones[] = {"CONTINUAR", "REINICIAR", "SALIR"};
    menuOpcionesDibujar(opciones, 3, centro_x, &y, seleccion, config);
}

void cubrirGameOverDibujar(tConfigPantalla* config, uint32_t score, uint16_t lineas, const char* nombre, uint8_t seleccion)
{
    if (!config) return;
    uint16_t ancho_panel = 320, alto_panel = 240;
    uint16_t x_panel = (config->ventana.ancho - ancho_panel) / 2, y_panel = (config->ventana.alto - alto_panel) / 2;
    rectanguloRelleno(0, 0, config->ventana.ancho, config->ventana.alto, N, config);
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, M, config);
    uint16_t centro_x = x_panel + ancho_panel / 2, y = y_panel + 25;
    textoCentradoDibujar("GAME OVER", centro_x, y, R, 3, config);
    y += ALTO_LETRA * 3 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 15;
    textoCentradoDibujar(nombre, centro_x, y, CC, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 10;
    textoCentradoDibujar("SCORE", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 5;
    {
        uint8_t digitos = contarDigitos(score), espaciado = 2;
        uint16_t ancho_valor = 0;
        for (uint8_t i = 0; i < digitos; i++) {
            uint8_t d = extraerDigito(score, digitos - 1 - i);
            ancho_valor += (ancho_digitos[d] + espaciado) * 2;
        }
        ancho_valor -= espaciado * 2;
        uint16_t x = centro_x - ancho_valor / 2;
    for (uint8_t i = 0; i < digitos; i++) {
        uint8_t d = extraerDigito(score, digitos - 1 - i);
        uint8_t ancho_d = ancho_digitos[d];
        hudDigitoDibujar(x, y, d, CC, 2, config);
        x += (ancho_d + espaciado) * 2;
    }
    }
    y += ALTO_NUMERO * 2 + 10;
    textoCentradoDibujar("LINEAS", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 5;
    {
        uint32_t lineas_32 = lineas;
        uint8_t digitos = contarDigitos(lineas_32), espaciado = 2;
        uint16_t ancho_valor = 0;
        for (uint8_t i = 0; i < digitos; i++) {
            uint8_t d = extraerDigito(lineas_32, digitos - 1 - i);
            ancho_valor += (ancho_digitos[d] + espaciado) * 2;
        }
        ancho_valor -= espaciado * 2;
        uint16_t x = centro_x - ancho_valor / 2;
        for (uint8_t i = 0; i < digitos; i++) {
            uint8_t d = extraerDigito(lineas_32, digitos - 1 - i);
            uint8_t ancho_d = ancho_digitos[d];
            hudDigitoDibujar(x, y, d, CC, 2, config);
            x += (ancho_d + espaciado) * 2;
        }
    }
    y += ALTO_NUMERO * 2 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 15;
    static const char* opciones[] = {"REINICIAR", "SALIR"};
    menuOpcionesDibujar(opciones, 2, centro_x, &y, seleccion, config);
}

void cubrirConfirmarSalidaDibujar(tConfigPantalla* config)
{
    if (!config) return;
    uint16_t ancho_panel = 260, alto_panel = 100;
    uint16_t x_panel = (config->ventana.ancho - ancho_panel) / 2, y_panel = (config->ventana.alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, NC, config);
    uint16_t centro_x = x_panel + ancho_panel / 2, centro_y = y_panel + alto_panel / 2 - 10;
    textoCentradoDibujar("SALIR?", centro_x, centro_y, NC, ETIQUETA_ESCALA, config);
    textoCentradoDibujar("S = Si   N = No", centro_x, centro_y + ALTO_LETRA * ETIQUETA_ESCALA + 12, NA, 1, config);
}

#define CANT_TETROMINOS_CAYENDO 8
#define TAM_BLOQUE_PRESENTACION 8
#define VELOCIDAD_CADA 2

static void fondoPresentacionDibujar(tConfigPantalla* config)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;

    static tTetromino cayendo[CANT_TETROMINOS_CAYENDO];
    static int16_t pos_x[CANT_TETROMINOS_CAYENDO];
    static int16_t pos_y[CANT_TETROMINOS_CAYENDO];
    static uint8_t inicializado = 0;
    static uint8_t frame_counter = 0;

    if (!inicializado) {
        for (uint8_t i = 0; i < CANT_TETROMINOS_CAYENDO; i++) {
            cayendo[i].tipo = (rand() % 7) + 1;
            cayendo[i].rotacion = rand() % 4;
            const uint8_t* matriz = tetrominoObtenerMatrizRotacion(cayendo[i].tipo, cayendo[i].rotacion);
            if (matriz)
                memcpy(cayendo[i].matriz, matriz, 16);
            tetrominoCalcularLimites(&cayendo[i]);
            pos_x[i] = rand() % (ancho - TAM_BLOQUE_PRESENTACION * 4);
            pos_y[i] = -(int16_t)(i * 50 + rand() % 30);
        }
        inicializado = 1;
    }

    frame_counter++;
    if (frame_counter >= VELOCIDAD_CADA) {
        for (uint8_t i = 0; i < CANT_TETROMINOS_CAYENDO; i++) {
            pos_y[i]++;
            if (pos_y[i] > (int16_t)(alto + 40)) {
                cayendo[i].tipo = (rand() % 7) + 1;
                cayendo[i].rotacion = rand() % 4;
                const uint8_t* matriz = tetrominoObtenerMatrizRotacion(cayendo[i].tipo, cayendo[i].rotacion);
                if (matriz)
                    memcpy(cayendo[i].matriz, matriz, 16);
                tetrominoCalcularLimites(&cayendo[i]);
                pos_x[i] = rand() % (ancho - TAM_BLOQUE_PRESENTACION * 4);
                pos_y[i] = -(int16_t)(TAM_BLOQUE_PRESENTACION * 4 + rand() % 30);
            }
        }
        frame_counter = 0;
    }

    for (uint16_t y = 0; y < alto; y++) {
        uint8_t color = (y < alto / 3) ? AO : ((y < alto * 2 / 3) ? N : N);
        lineaHDibujar(0, y, ancho, color, config);
    }

    for (uint16_t y = 0; y < alto; y += 3) {
        for (uint16_t x = (y / 3) % 2; x < ancho; x += 2) {
            pixelDibujar(x, y, AO, ancho, alto);
        }
    }

    for (uint8_t t = 0; t < CANT_TETROMINOS_CAYENDO; t++) {
        if (pos_y[t] + cayendo[t].max_fila * TAM_BLOQUE_PRESENTACION < 0)
            continue;
        if (pos_y[t] > (int16_t)alto)
            continue;
        for (uint8_t i = cayendo[t].min_fila; i <= cayendo[t].max_fila; i++) {
            for (uint8_t j = cayendo[t].min_col; j <= cayendo[t].max_col; j++) {
                if (cayendo[t].matriz[i][j] != 0) {
                    int16_t px = pos_x[t] + j * TAM_BLOQUE_PRESENTACION;
                    int16_t py = pos_y[t] + i * TAM_BLOQUE_PRESENTACION;
                    if (px >= 0 && py >= 0 && px < (int16_t)ancho && py < (int16_t)alto) {
                        for (uint8_t dy = 0; dy < TAM_BLOQUE_PRESENTACION; dy++)
                            for (uint8_t dx = 0; dx < TAM_BLOQUE_PRESENTACION; dx++)
                                pixelDibujar(px + dx, py + dy, cayendo[t].matriz[i][j], ancho, alto);
                    }
                }
            }
        }
    }

    rectanguloDibujar(2, 2, ancho - 4, alto - 4, CC, config);
    rectanguloDibujar(5, 5, ancho - 10, alto - 10, CO, config);
}

void presentarPantallaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    fondoPresentacionDibujar(config);

    uint8_t escala_titulo = (ancho >= 500) ? 4 : 2;
    uint8_t escala_sub = (ancho >= 500) ? 2 : 1;

    uint16_t ancho_titulo = hudTextoAncho("TETRIS", escala_titulo);
    uint16_t y_titulo = 30;

    uint16_t sombra_y = y_titulo + 3;
    hudPalabraDibujar("TETRIS", centro_x - ancho_titulo / 2 + 3, sombra_y, AO, escala_titulo, config);
    hudPalabraDibujar("TETRIS", centro_x - ancho_titulo / 2, y_titulo, NC, escala_titulo, config);

    uint16_t ancho_sub = hudTextoAncho("GRUPO NEXO", escala_sub);
    uint16_t y_sub = y_titulo + ALTO_LETRA * escala_titulo + 8;
    hudPalabraDibujar("GRUPO NEXO", centro_x - ancho_sub / 2, y_sub, CC, escala_sub, config);

    uint16_t y_info = y_sub + ALTO_LETRA * escala_sub + 12;
    hudPalabraDibujar("TRABAJO PRACTICO", centro_x - hudTextoAncho("TRABAJO PRACTICO", 1) / 2, y_info, NA, 1, config);
    y_info += ALTO_LETRA + 4;
    hudPalabraDibujar("2026", centro_x - hudTextoAncho("2026", 1) / 2, y_info, NA, 1, config);

    y_info += ALTO_LETRA + 8;
    lineaHDibujar(centro_x - 80, y_info, 160, M, config);

    static const char* opciones[] = {
        "START",
        "CARGAR PARTIDA",
        "PUNTAJES",
        "OPCIONES",
        "ACERCA DE",
        "SALIR"
    };

    uint16_t y_menu = y_info + 20;
    uint16_t ancho_opcion_max = 0;
    for (uint8_t i = 0; i < PRESENTACION_CANT_OPCIONES; i++) {
        uint16_t ancho_op = hudTextoAncho(opciones[i], ETIQUETA_ESCALA);
        if (ancho_op > ancho_opcion_max) ancho_opcion_max = ancho_op;
    }

    uint16_t alto_opcion = ALTO_LETRA * ETIQUETA_ESCALA + 12;
    uint16_t panel_ancho = ancho_opcion_max + 40;
    uint16_t panel_x = centro_x - panel_ancho / 2;

    for (uint8_t i = 0; i < PRESENTACION_CANT_OPCIONES; i++) {
        uint16_t y_op = y_menu + i * (alto_opcion + 4);
        uint16_t ancho_op = hudTextoAncho(opciones[i], ETIQUETA_ESCALA);
        uint16_t x_texto = centro_x - ancho_op / 2;

        if (i == opcion_seleccionada) {
            rectanguloRelleno(panel_x, y_op - 4, panel_ancho, alto_opcion, AO, config);
            rectanguloDibujar(panel_x, y_op - 4, panel_ancho, alto_opcion, CC, config);
            hudPalabraDibujar(">", panel_x + 6, y_op + 2, CC, ETIQUETA_ESCALA, config);
            hudPalabraDibujar(opciones[i], x_texto + 8, y_op + 2, CC, ETIQUETA_ESCALA, config);
        } else {
            hudPalabraDibujar(opciones[i], x_texto, y_op + 2, NA, ETIQUETA_ESCALA, config);
        }
    }

    uint16_t y_pie = alto - 30;
    hudPalabraDibujar("TOPICOS DE PROGRAMACION - 2026",
        centro_x - hudTextoAncho("TOPICOS DE PROGRAMACION - 2026", 1) / 2,
        y_pie, CO, 1, config);

    hudPalabraDibujar("Flechas: navegar | ENTER: seleccionar | ESC: salir",
        centro_x - hudTextoAncho("Flechas: navegar | ENTER: seleccionar | ESC: salir", 1) / 2,
        y_pie + ALTO_LETRA + 4, CO, 1, config);
}

void menuConfiguracionDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada, uint8_t modalidad, uint8_t columnas_dx, double velocidad, uint8_t confirmacion_guardado)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    rectanguloRelleno(0, 0, ancho, alto, N, config);

    uint16_t ancho_panel = 380, alto_panel = 320;
    uint16_t x_panel = (ancho - ancho_panel) / 2, y_panel = (alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, CC, config);

    uint16_t y = y_panel + 20;
    textoCentradoDibujar("OPCIONES", centro_x, y, CC, 3, config);
    y += ALTO_LETRA * 3 + 10;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 20;

    static const char* etiquetas[] = {
        "MODALIDAD",
        "COLUMNAS",
        "VELOCIDAD INICIAL",
        "GUARDAR CONFIG",
        "VOLVER"
    };

    char valores[5][32];

    snprintf(valores[0], sizeof(valores[0]), "%s", modalidad == 0 ? "CLASICO" : "DELUXE");
    snprintf(valores[1], sizeof(valores[1]), "%d", (int)columnas_dx);
    snprintf(valores[2], sizeof(valores[2]), "%.2fS", velocidad);
    valores[3][0] = '\0';
    valores[4][0] = '\0';

    uint16_t alto_opcion = ALTO_LETRA * ETIQUETA_ESCALA + 16;
    uint16_t separacion = 8;

    for (uint8_t i = 0; i < OPCIONES_CANT_OPCIONES; i++) {
        uint16_t y_op = y + i * (alto_opcion + separacion);
        uint16_t ancho_etiq = hudTextoAncho(etiquetas[i], ETIQUETA_ESCALA);
        uint16_t x_etiq = centro_x - ancho_etiq / 2 - 30;

        if (i == opcion_seleccionada) {
            uint16_t ancho_valor = hudTextoAncho(valores[i], ETIQUETA_ESCALA);
            uint16_t ancho_total = ancho_etiq + 40 + ancho_valor;
            uint16_t x_panel_op = centro_x - ancho_total / 2;
            rectanguloRelleno(x_panel_op, y_op - 6, ancho_total, alto_opcion, AO, config);
            rectanguloDibujar(x_panel_op, y_op - 6, ancho_total, alto_opcion, CC, config);
        }

        hudPalabraDibujar(etiquetas[i], x_etiq, y_op, i == opcion_seleccionada ? CC : NA, ETIQUETA_ESCALA, config);

        if (valores[i][0] != '\0') {
            uint16_t x_val = x_etiq + ancho_etiq + 20;
            hudPalabraDibujar(valores[i], x_val, y_op, i == opcion_seleccionada ? CC : CO, ETIQUETA_ESCALA, config);
        }

        if (i == OPCIONES_OPCION_GUARDAR && confirmacion_guardado > 0) {
            uint16_t x_val = x_etiq + ancho_etiq + 20;
            if (confirmacion_guardado == 255) {
                hudPalabraDibujar("ERROR", x_val, y_op, R, ETIQUETA_ESCALA, config);
            } else {
                hudPalabraDibujar("OK", x_val, y_op, V, ETIQUETA_ESCALA, config);
            }
        }

        if (i == opcion_seleccionada) {
            if (i == OPCIONES_OPCION_MODALIDAD || i == OPCIONES_OPCION_VELOCIDAD ||
                (i == OPCIONES_OPCION_COLUMNAS && modalidad == 1)) {
                uint16_t x_flecha = centro_x + 160;
                hudPalabraDibujar("<", x_flecha - 20, y_op, CC, ETIQUETA_ESCALA, config);
                hudPalabraDibujar(">", x_flecha + 10, y_op, CC, ETIQUETA_ESCALA, config);
            }
        }
    }

    uint16_t y_pie = y_panel + alto_panel - 20;
    hudPalabraDibujar("Flechas cambiar ENTER confirmar ESC volver",
        centro_x - hudTextoAncho("Flechas cambiar ENTER confirmar ESC volver", 1) / 2,
        y_pie, CO, 1, config);
}

void pantallaAcercaDeDibujar(tConfigPantalla* config)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    rectanguloRelleno(0, 0, ancho, alto, N, config);

    uint16_t ancho_panel = 360, alto_panel = 260;
    uint16_t x_panel = (ancho - ancho_panel) / 2, y_panel = (alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, M, config);

    uint16_t y = y_panel + 20;
    textoCentradoDibujar("ACERCA DE", centro_x, y, M, 3, config);
    y += ALTO_LETRA * 3 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 20;

    textoCentradoDibujar("TETRIS - GRUPO NEXO", centro_x, y, CC, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 10;
    textoCentradoDibujar("TRABAJO PRACTICO", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 6;
    textoCentradoDibujar("TOPICOS DE PROGRAMACION", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 6;
    textoCentradoDibujar("2026", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 20;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 15;
    textoCentradoDibujar("Presiona ESC para volver", centro_x, y, CO, 1, config);
}

void pantallaCargarPartidaDibujar(tConfigPantalla* config)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    rectanguloRelleno(0, 0, ancho, alto, N, config);

    uint16_t ancho_panel = 320, alto_panel = 160;
    uint16_t x_panel = (ancho - ancho_panel) / 2, y_panel = (alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, AO, config);

    uint16_t y = y_panel + 25;
    textoCentradoDibujar("CARGAR PARTIDA", centro_x, y, CC, 3, config);
    y += ALTO_LETRA * 3 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 20;
    textoCentradoDibujar("Funcionalidad pendiente", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 15;
    textoCentradoDibujar("Presiona ESC para volver", centro_x, y, CO, 1, config);
}

void pantallaPuntajesDibujar(tConfigPantalla* config, tRanking* rank, uint8_t confirmar_borrar)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    rectanguloRelleno(0, 0, ancho, alto, N, config);

    uint16_t ancho_panel = 380, alto_panel;
    size_t cant_entradas = rank ? ranking_tamanio(rank) : 0;
    alto_panel = (cant_entradas > 0) ? (uint16_t)(80 + cant_entradas * 25) : 200;
    if (alto_panel > alto - 40) alto_panel = alto - 40;

    uint16_t x_panel = (ancho - ancho_panel) / 2, y_panel = (alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, AM, config);

    uint16_t y = y_panel + 20;
    textoCentradoDibujar("PUNTAJES", centro_x, y, AM, 3, config);
    y += ALTO_LETRA * 3 + 10;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 15;

    if (cant_entradas == 0) {
        textoCentradoDibujar("No hay puntajes registrados", centro_x, y, NA, ETIQUETA_ESCALA, config);
        y += ALTO_LETRA * ETIQUETA_ESCALA + 10;
        textoCentradoDibujar("Completaste una partida para figurar", centro_x, y, CO, 1, config);
    } else {
        for (size_t i = 0; i < cant_entradas; i++) {
            tEntradaRanking* entrada = ranking_obtener(rank, i);
            if (!entrada) continue;

            char linea[80];
            snprintf(linea, sizeof(linea), "%zu. %s | %lu | %s",
                     i + 1, entrada->nombre,
                     (unsigned long)entrada->puntos,
                     entrada->fecha_hora);

            if (i == 0) {
                hudPalabraDibujar("*", x_panel + 25, y, AM, ETIQUETA_ESCALA, config);
            }

            hudPalabraDibujar(linea, x_panel + 45, y, i == 0 ? AM : CC, ETIQUETA_ESCALA, config);

            y += 22;
        }
    }

    y = y_panel + alto_panel - 20;
    if (confirmar_borrar) {
        textoCentradoDibujar("Borrar todo? S = Si | N = No", centro_x, y, R, ETIQUETA_ESCALA, config);
    } else {
        textoCentradoDibujar("R: borrar | ESC: volver", centro_x, y, CO, 1, config);
    }
}

void pantallaIngresarNombreDibujar(tConfigPantalla* config, const char* buffer, uint8_t cursor_pos, bool cursor_visible)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    rectanguloRelleno(0, 0, ancho, alto, N, config);

    uint16_t ancho_panel = 340, alto_panel = 140;
    uint16_t x_panel = (ancho - ancho_panel) / 2, y_panel = (alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, CC, config);

    uint16_t y = y_panel + 20;
    textoCentradoDibujar("INGRESE SU NOMBRE", centro_x, y, CC, 2, config);
    y += ALTO_LETRA * 2 + 20;

    uint16_t x_input = x_panel + 30;
    uint16_t y_input = y;
    uint16_t ancho_input = ancho_panel - 60;
    uint16_t alto_input = ALTO_LETRA * 2 + 10;

    rectanguloRelleno(x_input, y_input, ancho_input, alto_input, AO, config);
    rectanguloDibujar(x_input, y_input, ancho_input, alto_input, CC, config);

    uint16_t x_cursor = x_input + 5;
    for (uint8_t i = 0; buffer[i] != '\0' && i < cursor_pos; i++) {
        uint8_t ancho_c = hudCaracterAncho(buffer[i]);
        if (buffer[i] == ' ') {
            x_cursor += 4;
        } else {
            hudLetraDibujar(x_cursor, y_input + 3, buffer[i], CC, 2, config);
            x_cursor += ancho_c * 2 + 1;
        }
    }

    if (cursor_visible) {
        rectanguloRelleno(x_cursor, y_input + 2, 2, alto_input - 6, CC, config);
    }

    y += alto_input + 15;
    textoCentradoDibujar("A-Z 0-9 Espacio | Backspace: borrar | Enter: confirmar",
        centro_x, y, CO, 1, config);
}

void pantallaMensajeDibujar(tConfigPantalla* config, const char* titulo, const char* mensaje)
{
    if (!config) return;

    uint16_t ancho = config->ventana.ancho, alto = config->ventana.alto;
    uint16_t centro_x = ancho / 2;

    rectanguloRelleno(0, 0, ancho, alto, N, config);

    uint16_t ancho_panel = 300, alto_panel = 120;
    uint16_t x_panel = (ancho - ancho_panel) / 2, y_panel = (alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, CC, config);

    uint16_t y = y_panel + 20;
    textoCentradoDibujar(titulo, centro_x, y, CC, 2, config);
    y += ALTO_LETRA * 2 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M, config);
    y += 15;
    textoCentradoDibujar(mensaje, centro_x, y, NA, ETIQUETA_ESCALA, config);
}