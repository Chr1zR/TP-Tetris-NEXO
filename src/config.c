#include "../include/config.h"
#include "../include/hud.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @desc Inicializa valores default de configuracion.
 * @param c tConfigPantalla* Puntero a la configuracion.
 * @return void
 */
static void configValoresDefault(tConfigPantalla* c)
{
    c->ventana.ancho = 640;
    c->ventana.alto = 480;
    c->ventana.escala = 1;
    c->tablero.filas = 20 + FILAS_INVISIBLES;
    c->tablero.filas_visibles = 20;
    c->tablero.columnas = 10;
    c->modo_juego = MODO_CLASICO;
    c->velocidad_inicial = 1.0;
    c->tablero_circular = false;
    c->piezas_extras = false;
    c->cheats_habilitados = false;
}

void configGeometriaRecalcular(tConfigPantalla* c)
{
    c->tablero.tam_bloque = (c->ventana.alto - 6) / c->tablero.filas_visibles;
    c->tablero.alto_px = c->tablero.filas_visibles * c->tablero.tam_bloque;
    anchosCalcular(c);
}

/**
 * @desc Valida que los valores de configuracion sean correctos.
 * @param c const tConfigPantalla* Puntero a la configuracion.
 * @return bool true si valida, false si no.
 */
static bool configValidar(const tConfigPantalla* c)
{
    if (c->ventana.ancho == 0 || c->ventana.alto == 0)
        return false;
    if (c->ventana.escala == 0)
        return false;
    if (c->tablero.columnas < 8 || c->tablero.columnas > 16)
        return false;
    if (c->tablero.filas_visibles == 0)
        return false;
    if (c->tablero.filas < c->tablero.filas_visibles)
        return false;
    if (c->velocidad_inicial <= 0.0 || c->velocidad_inicial > 2.0)
        return false;
    return true;
}

tConfigPantalla* crear_config(int argc, char* argv[])
{
    if (argc != 1 && argc != 3)
        return NULL;
    if (argc == 3 && (!argv || !argv[1] || !argv[2]))
        return NULL;

    tConfigPantalla* c = calloc(1, sizeof(*c));
    if (!c)
        return NULL;

    configValoresDefault(c);

    if (argc == 3) {
        int escala = atoi(argv[2]);
        if (escala <= 0) {
            free(c);
            return NULL;
        }
        c->ventana.escala = (uint8_t)escala;

        if (strcmp(argv[1], "VGA") == 0) {
            c->ventana.ancho = 640;
            c->ventana.alto = 480;
        } else if (strcmp(argv[1], "CGA") == 0) {
            c->ventana.ancho = 320;
            c->ventana.alto = 200;
        } else {
            free(c);
            return NULL;
        }
    }

    configGeometriaRecalcular(c);
    return c;
}

void configDestruir(tConfigPantalla* config)
{
    if (config)
        free(config);
}

void anchosCalcular(tConfigPantalla* config)
{
    if (!config)
        return;

    config->tablero.ancho_px = config->tablero.columnas * config->tablero.tam_bloque;
    config->tablero.offset_x = (config->ventana.ancho - config->tablero.ancho_px) / 2;
    config->tablero.offset_y = (config->ventana.alto - config->tablero.alto_px) / 2;
    config->hud.ancho_izq = config->tablero.offset_x;
    config->hud.ancho_der = config->ventana.ancho - config->tablero.offset_x - config->tablero.ancho_px;

    disposicionHudCalcular(config);
}

int configGuardar(tConfigPantalla* config, const char* ruta)
{
    if (!config || !ruta)
        return -1;

    FILE* f = fopen(ruta, "w");
    if (!f)
        return -1;

    fprintf(f, "resolucion=%s\n", config->ventana.ancho >= 640 ? "VGA" : "CGA");
    fprintf(f, "escala=%d\n", config->ventana.escala);
    fprintf(f, "modo=%d\n", config->modo_juego);
    fprintf(f, "columnas=%d\n", config->tablero.columnas);
    fprintf(f, "velocidad=%.2f\n", config->velocidad_inicial);
    fprintf(f, "circular=%d\n", config->tablero_circular ? 1 : 0);
    fprintf(f, "extras=%d\n", config->piezas_extras ? 1 : 0);
    fprintf(f, "cheats=%d\n", config->cheats_habilitados ? 1 : 0);

    fclose(f);
    return 0;
}

int configCargar(tConfigPantalla* config, const char* ruta)
{
    if (!config || !ruta)
        return -1;

    tConfigPantalla tmp;
    configValoresDefault(&tmp);

    FILE* f = fopen(ruta, "r");
    if (!f)
        return -1;

    char clave[64], valor[64];
    while (fscanf(f, "%63[^=]=%63[^\n]\n", clave, valor) == 2) {
        size_t len = strlen(valor);
        while (len > 0 && (valor[len - 1] == '\r' || valor[len - 1] == '\n' || valor[len - 1] == ' ')) {
            valor[--len] = '\0';
        }
        if (strcmp(clave, "resolucion") == 0) {
            if (strcmp(valor, "VGA") == 0) {
                tmp.ventana.ancho = 640;
                tmp.ventana.alto = 480;
            } else {
                tmp.ventana.ancho = 320;
                tmp.ventana.alto = 200;
            }
        } else if (strcmp(clave, "escala") == 0) {
            tmp.ventana.escala = (uint8_t)atoi(valor);
        } else if (strcmp(clave, "modo") == 0) {
            tmp.modo_juego = (tModoJuego)atoi(valor);
        } else if (strcmp(clave, "columnas") == 0) {
            tmp.tablero.columnas = (uint8_t)atoi(valor);
        } else if (strcmp(clave, "velocidad") == 0) {
            tmp.velocidad_inicial = atof(valor);
        } else if (strcmp(clave, "circular") == 0) {
            tmp.tablero_circular = (atoi(valor) != 0);
        } else if (strcmp(clave, "extras") == 0) {
            tmp.piezas_extras = (atoi(valor) != 0);
        } else if (strcmp(clave, "cheats") == 0) {
            tmp.cheats_habilitados = (atoi(valor) != 0);
        }
    }

    fclose(f);

    tmp.tablero.filas = 20 + FILAS_INVISIBLES;
    tmp.tablero.filas_visibles = 20;
    tmp.tablero.tam_bloque = (tmp.ventana.alto - 6) / tmp.tablero.filas_visibles;
    tmp.tablero.alto_px = tmp.tablero.filas_visibles * tmp.tablero.tam_bloque;

    if (!configValidar(&tmp))
        return -1;

    anchosCalcular(&tmp);
    *config = tmp;
    return 0;
}
