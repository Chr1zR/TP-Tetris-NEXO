#include "../include/guardar.h"
#include "../include/plataforma.h"
#include "../include/config.h"
#include "../include/logica.h"
#include <stdio.h>
#include <string.h>

int partidaGuardar(tJuego* juego, const char* ruta)
{
    if(!juego || !juego->config || !juego->tablero || !juego->estado || !ruta)
        return -1;

    FILE* f = fopen(ruta, "wb");
    if(!f)
        return -1;

    int ok = 0;
    uint32_t magic = PARTIDA_MAGIC;
    uint16_t version = PARTIDA_VERSION;

    if(ok == 0 && fwrite(&magic, sizeof(magic), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(&version, sizeof(version), 1, f) != 1) ok = -1;

    if(ok == 0 && fwrite(juego->config, sizeof(tConfigPantalla), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(juego->estado, sizeof(tEstadoJuego), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(juego->tablero->pieza_actual, sizeof(tTetromino), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(juego->tablero->pieza_siguiente, sizeof(tTetromino), 1, f) != 1) ok = -1;

    if(ok == 0) {
        for(uint8_t i = 0; i < juego->config->tablero.filas; i++) {
            if(fwrite(juego->tablero->tablero[i], sizeof(unsigned char),
                      juego->config->tablero.columnas, f) != juego->config->tablero.columnas) {
                ok = -1;
                break;
            }
        }
    }

    if(ok == 0 && fwrite(juego->nombre_jugador, sizeof(juego->nombre_jugador), 1, f) != 1) ok = -1;

    uint16_t seg = juego->seg_transcurrido;
    double intervalo = juego->intervalo_actual;
    uint8_t tolerancia = juego->en_tolerancia_fijacion ? 1 : 0;
    double intervalo_fij = juego->intervalo_fijacion;

    if(ok == 0 && fwrite(&seg, sizeof(seg), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(&intervalo, sizeof(intervalo), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(&tolerancia, sizeof(tolerancia), 1, f) != 1) ok = -1;
    if(ok == 0 && fwrite(&intervalo_fij, sizeof(intervalo_fij), 1, f) != 1) ok = -1;

    fclose(f);
    return ok;
}

bool partidaExiste(const char* ruta)
{
    FILE* f = fopen(ruta, "rb");
    if(!f)
        return false;
    fclose(f);
    return true;
}

int partidaLeerInfo(const char* ruta, tInfoPartida* info)
{
    if(!ruta || !info)
        return -1;

    FILE* f = fopen(ruta, "rb");
    if(!f)
        return -1;

    uint32_t magic;
    uint16_t version;
    if(fread(&magic, sizeof(magic), 1, f) != 1) { fclose(f); return -1; }
    if(fread(&version, sizeof(version), 1, f) != 1) { fclose(f); return -1; }
    if(magic != PARTIDA_MAGIC || version != PARTIDA_VERSION) { fclose(f); return -2; }

    tConfigPantalla cfg;
    tEstadoJuego estado;
    if(fread(&cfg, sizeof(cfg), 1, f) != 1) { fclose(f); return -1; }
    if(fread(&estado, sizeof(estado), 1, f) != 1) { fclose(f); return -1; }

    if(fseek(f, sizeof(tTetromino) * 2, SEEK_CUR) != 0) { fclose(f); return -1; }

    long board_offset = (long)cfg.tablero.filas * (long)cfg.tablero.columnas;
    if(fseek(f, board_offset, SEEK_CUR) != 0) { fclose(f); return -1; }

    char nombre[sizeof(info->nombre)];
    uint16_t seg;
    if(fread(nombre, sizeof(nombre), 1, f) != 1) { fclose(f); return -1; }
    if(fread(&seg, sizeof(seg), 1, f) != 1) { fclose(f); return -1; }

    fclose(f);

    memcpy(info->nombre, nombre, sizeof(nombre));
    info->puntos = estado.puntos;
    info->nivel = estado.nivel;
    info->segundos = seg;
    info->lineas = estado.lineas_limpias;
    return 0;
}

int partidaCargar(tJuego* juego, const char* ruta)
{
    if(!juego || !juego->config || !juego->tablero || !juego->estado || !ruta)
        return -1;

    FILE* f = fopen(ruta, "rb");
    if(!f)
        return -1;

    int ok = 0;
    uint32_t magic;
    uint16_t version;

    if(fread(&magic, sizeof(magic), 1, f) != 1) ok = -1;
    if(fread(&version, sizeof(version), 1, f) != 1) ok = -1;

    if(ok == 0 && magic != PARTIDA_MAGIC) ok = -2;
    if(ok == 0 && version != PARTIDA_VERSION) ok = -2;

    tConfigPantalla config_guardada;
    if(ok == 0) {
        if(fread(&config_guardada, sizeof(tConfigPantalla), 1, f) != 1) ok = -1;
    }

    if(ok == 0) {
        bool resolucion_difiere = (config_guardada.ventana.ancho != juego->config->ventana.ancho ||
                                   config_guardada.ventana.alto != juego->config->ventana.alto ||
                                   config_guardada.ventana.escala != juego->config->ventana.escala);
        bool dimensiones_difieren = (config_guardada.tablero.filas != juego->config->tablero.filas ||
                                     config_guardada.tablero.columnas != juego->config->tablero.columnas);

        if(resolucion_difiere) {
            juego->config->ventana = config_guardada.ventana;
            if(reiniciar_plataforma(juego->config) != 0) {
                ok = -4;
            }
        }

        if(ok == 0 && dimensiones_difieren) {
            tTetromino_Destruir(juego->tablero->pieza_actual);
            tTetromino_Destruir(juego->tablero->pieza_siguiente);
            vaciar_tablero(juego->tablero, juego->config);
            free(juego->tablero);

            juego->config->tablero.filas = config_guardada.tablero.filas;
            juego->config->tablero.filas_visibles = config_guardada.tablero.filas_visibles;
            juego->config->tablero.columnas = config_guardada.tablero.columnas;

            juego->tablero = crear_tablero(juego->config);
            if(!juego->tablero) {
                ok = -5;
            } else {
                tTetromino_Destruir(juego->tablero->pieza_actual);
                tTetromino_Destruir(juego->tablero->pieza_siguiente);
            }
        }

        if(ok == 0) {
            configGeometriaRecalcular(juego->config);
        }
    }

    if(ok == 0) {
        juego->config->modo_juego = config_guardada.modo_juego;
        juego->config->velocidad_inicial = config_guardada.velocidad_inicial;
        juego->config->tablero_circular = config_guardada.tablero_circular;
        juego->config->piezas_extras = config_guardada.piezas_extras;
        juego->config->cheats_habilitados = config_guardada.cheats_habilitados;
    }

    if(ok == 0) {
        if(fread(juego->estado, sizeof(tEstadoJuego), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        if(fread(juego->tablero->pieza_actual, sizeof(tTetromino), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        if(fread(juego->tablero->pieza_siguiente, sizeof(tTetromino), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        for(uint8_t i = 0; i < config_guardada.tablero.filas; i++) {
            if(fread(juego->tablero->tablero[i], sizeof(unsigned char),
                     config_guardada.tablero.columnas, f) != config_guardada.tablero.columnas)
                ok = -1;
        }
    }

    char nombre[sizeof(juego->nombre_jugador)];
    uint16_t seg;
    double intervalo;
    uint8_t tolerancia;
    double intervalo_fij;

    if(ok == 0) {
        if(fread(nombre, sizeof(nombre), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        if(fread(&seg, sizeof(seg), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        if(fread(&intervalo, sizeof(intervalo), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        if(fread(&tolerancia, sizeof(tolerancia), 1, f) != 1) ok = -1;
    }
    if(ok == 0) {
        if(fread(&intervalo_fij, sizeof(intervalo_fij), 1, f) != 1) ok = -1;
    }

    fclose(f);

    if(ok != 0)
        return ok;

    memcpy(juego->nombre_jugador, nombre, sizeof(nombre));
    juego->seg_transcurrido = seg;
    juego->intervalo_actual = intervalo;
    juego->en_tolerancia_fijacion = (tolerancia != 0);
    juego->intervalo_fijacion = intervalo_fij;

    gbt_temporizador_destruir(juego->temporizador);
    juego->temporizador = gbt_temporizador_crear(juego->intervalo_actual);

    gbt_temporizador_destruir(juego->temporizador_seg);
    juego->temporizador_seg = gbt_temporizador_crear(1.0);

    if(juego->temporizador_fijacion) {
        gbt_temporizador_destruir(juego->temporizador_fijacion);
        juego->temporizador_fijacion = NULL;
    }
    if(juego->en_tolerancia_fijacion) {
        juego->temporizador_fijacion = gbt_temporizador_crear(juego->intervalo_fijacion);
    }

    juego->game_over = false;
    juego->redibujar_hud = 1;

    return 0;
}
