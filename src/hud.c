#include "../include/hud.h"
#include "../include/config.h"
#include "../include/piezas.h"
#include "../include/dibujar.h"
#include <GBT/gbt.h>

#define MARGEN 10
#define ALTO_CAJA 36
#define ETIQUETA_DY 3
#define ETIQUETA_ESCALA 2
#define ETIQUETA_ALTO 15

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
            cajas[i].offset_y_final - cajas[i].offset_y_inicial, NA);
}

void hudPalabraDibujar(const char* palabra, uint16_t pos_x, uint16_t pos_y, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    if (!config || !palabra) return;
    const uint8_t (*letras[26])[ANCHO_LETRA] = {
        letra_A, letra_B, letra_C, letra_D, letra_E, letra_F, letra_G, letra_H, letra_I, letra_J,
        letra_K, letra_L, letra_M, letra_N, letra_O, letra_P, letra_Q, letra_R, letra_S, letra_T,
        letra_U, letra_V, letra_W, letra_X, letra_Y, letra_Z
    };
    uint16_t paso = ANCHO_LETRA * escala + 1;
    for (int i = 0; palabra[i]; i++) {
        char c = palabra[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            uint8_t indice = (c >= 'a') ? c - 'a' : c - 'A';
            bitmapDibujar(&letras[indice][0][0], pos_x, pos_y, ANCHO_LETRA, ALTO_LETRA, color, escala);
        }
        pos_x += paso;
    }
}

static uint16_t hudTextoAncho(const char* texto, uint8_t escala)
{
    if (!texto || !texto[0]) return 0;
    uint16_t cant = 0;
    while (texto[cant]) cant++;
    return (uint16_t)(cant * (ANCHO_LETRA * escala + 1)) - 1;
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

static inline void hudNumeroDibujar(uint16_t x, uint16_t y, uint8_t numero, uint8_t color, uint8_t escala, tConfigPantalla* config)
{
    (void)config;
    if (numero > 9) return;
    static const uint8_t (*numeros[10])[ANCHO_NUMERO] = {
        cero, uno, dos, tres, cuatro, cinco, seis, siete, ocho, nueve
    };
    bitmapDibujar(&numeros[numero][0][0], x, y, ANCHO_NUMERO, ALTO_NUMERO, color, escala);
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
    uint16_t ancho_valor = (digitos * (ANCHO_NUMERO + espaciado) - espaciado) * escala;
    uint16_t ancho_caja = caja.offset_x_final - caja.offset_x_inicial;
    uint16_t alto_caja = caja.offset_y_final - caja.offset_y_inicial - ETIQUETA_ALTO;
    uint16_t x = caja.offset_x_inicial + (ancho_caja - ancho_valor) / 2;
    uint16_t y = caja.offset_y_inicial + ETIQUETA_ALTO + (alto_caja - ALTO_NUMERO * escala) / 2;
    for (uint8_t i = digitos; i > 0; i--) {
        hudNumeroDibujar(x, y, extraerDigito(valor, i - 1), CC, escala, config);
        x += (ANCHO_NUMERO + espaciado) * escala;
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
    uint16_t ancho_total = (4 * ANCHO_NUMERO + 2 * espaciado + 2) * escala;
    uint16_t ancho_caja = caja.offset_x_final - caja.offset_x_inicial;
    uint16_t alto_caja = caja.offset_y_final - caja.offset_y_inicial - ETIQUETA_ALTO;
    uint16_t x = caja.offset_x_inicial + (ancho_caja - ancho_total) / 2;
    uint16_t y = caja.offset_y_inicial + ETIQUETA_ALTO + (alto_caja - ALTO_NUMERO * escala) / 2;
    hudNumeroDibujar(x, y, minutos / 10, CC, escala, config); x += (ANCHO_NUMERO + espaciado) * escala;
    hudNumeroDibujar(x, y, minutos % 10, CC, escala, config); x += (ANCHO_NUMERO + espaciado) * escala;
    hudDosPuntosDibujar(x + escala, y, CC, escala);           x += (2 + espaciado) * escala;
    hudNumeroDibujar(x, y, seg_restantes / 10, CC, escala, config); x += (ANCHO_NUMERO + espaciado) * escala;
    hudNumeroDibujar(x, y, seg_restantes % 10, CC, escala, config);
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
    uint16_t ancho_num = 3 * (ANCHO_NUMERO + 1) - 1;
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
        for (uint8_t i = digitos; i > 0; i--)
            hudNumeroDibujar(x_num_base + (digitos - i) * (ANCHO_NUMERO + 1), y_num,
                extraerDigito(conteo_pieza, i - 1), CC, 1, config);
    }
}

void hudTituloNeonDibujar(tConfigPantalla* config)
{
    if (!config) return;
    uint8_t escala_titulo = (config->ventana.ancho >= 400) ? 3 : 1;
    uint8_t escala_sub = (config->ventana.ancho >= 400) ? 2 : 1;
    uint16_t ancho_panel = config->hud.ancho_izq;
    uint16_t ancho_titulo = 6 * (ANCHO_LETRA * escala_titulo + 1) - 1;
    uint16_t ancho_sub = 10 * (ANCHO_LETRA * escala_sub + 1) - 1;
    uint16_t y_titulo = config->hud.disposicion.piezas.offset_y_final + 12;
    uint16_t y_sub = y_titulo + ALTO_LETRA * escala_titulo + 5;
    hudPalabraDibujar("TETRIS", (ancho_panel > ancho_titulo ? (ancho_panel - ancho_titulo) / 2 : 0), y_titulo, NC, escala_titulo, config);
    hudPalabraDibujar("GRUPO NEXO", (ancho_panel > ancho_sub ? (ancho_panel - ancho_sub) / 2 : 0), y_sub, NA, escala_sub, config);
}

static void panelDibujar(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint8_t color)
{
    rectanguloRelleno(x, y, ancho, alto, N);
    rectanguloDibujar(x, y, ancho, alto, color);
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
            rectanguloRelleno(x_opcion, y_opcion, ancho_opcion + 12, ALTO_LETRA * ETIQUETA_ESCALA + 12, AO);
            rectanguloDibujar(x_opcion, y_opcion, ancho_opcion + 12, ALTO_LETRA * ETIQUETA_ESCALA + 12, CC);
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
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, C);
    uint16_t centro_x = x_panel + ancho_panel / 2, y = y_panel + 25;
    textoCentradoDibujar("PAUSA", centro_x, y, CC, 3, config);
    y += ALTO_LETRA * 3 + 10;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M);
    y += 15;
    static const char* opciones[] = {"CONTINUAR", "REINICIAR", "SALIR"};
    menuOpcionesDibujar(opciones, 3, centro_x, &y, seleccion, config);
}

void cubrirGameOverDibujar(tConfigPantalla* config, uint32_t score, uint16_t lineas, const char* nombre, uint8_t seleccion)
{
    if (!config) return;
    uint16_t ancho_panel = 320, alto_panel = 240;
    uint16_t x_panel = (config->ventana.ancho - ancho_panel) / 2, y_panel = (config->ventana.alto - alto_panel) / 2;
    rectanguloRelleno(0, 0, config->ventana.ancho, config->ventana.alto, N);
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, M);
    uint16_t centro_x = x_panel + ancho_panel / 2, y = y_panel + 25;
    textoCentradoDibujar("GAME OVER", centro_x, y, R, 3, config);
    y += ALTO_LETRA * 3 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M);
    y += 15;
    textoCentradoDibujar(nombre, centro_x, y, CC, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 10;
    textoCentradoDibujar("SCORE", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 5;
    {
        uint8_t digitos = contarDigitos(score), espaciado = 2;
        uint16_t ancho_valor = (digitos * (ANCHO_NUMERO + espaciado) - espaciado) * 2;
        uint16_t x = centro_x - ancho_valor / 2;
        for (uint8_t i = digitos; i > 0; i--) {
            hudNumeroDibujar(x, y, extraerDigito(score, i - 1), CC, 2, config);
            x += (ANCHO_NUMERO + espaciado) * 2;
        }
    }
    y += ALTO_NUMERO * 2 + 10;
    textoCentradoDibujar("LINEAS", centro_x, y, NA, ETIQUETA_ESCALA, config);
    y += ALTO_LETRA * ETIQUETA_ESCALA + 5;
    {
        uint32_t lineas_32 = lineas;
        uint8_t digitos = contarDigitos(lineas_32), espaciado = 2;
        uint16_t ancho_valor = (digitos * (ANCHO_NUMERO + espaciado) - espaciado) * 2;
        uint16_t x = centro_x - ancho_valor / 2;
        for (uint8_t i = digitos; i > 0; i--) {
            hudNumeroDibujar(x, y, extraerDigito(lineas_32, i - 1), CC, 2, config);
            x += (ANCHO_NUMERO + espaciado) * 2;
        }
    }
    y += ALTO_NUMERO * 2 + 15;
    lineaHDibujar(x_panel + 20, y, ancho_panel - 40, M);
    y += 15;
    static const char* opciones[] = {"REINICIAR", "SALIR"};
    menuOpcionesDibujar(opciones, 2, centro_x, &y, seleccion, config);
}

void cubrirConfirmarSalidaDibujar(tConfigPantalla* config)
{
    if (!config) return;
    uint16_t ancho_panel = 260, alto_panel = 100;
    uint16_t x_panel = (config->ventana.ancho - ancho_panel) / 2, y_panel = (config->ventana.alto - alto_panel) / 2;
    panelDibujar(x_panel, y_panel, ancho_panel, alto_panel, NC);
    uint16_t centro_x = x_panel + ancho_panel / 2, centro_y = y_panel + alto_panel / 2 - 10;
    textoCentradoDibujar("SALIR?", centro_x, centro_y, NC, ETIQUETA_ESCALA, config);
    textoCentradoDibujar("S = Si   N = No", centro_x, centro_y + ALTO_LETRA * ETIQUETA_ESCALA + 12, NA, 1, config);
}
