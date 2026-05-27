#include "../include/ranking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char SEPARADOR = '|';

static void formatear_fecha_hora(char* buffer, size_t tam_buffer)
{
    time_t ahora = time(NULL);
    struct tm* info_tiempo = localtime(&ahora);
    strftime(buffer, tam_buffer, "%d/%m/%Y %H:%M", info_tiempo);
}

tRanking* ranking_crear(size_t max_entradas)
{
    if (max_entradas == 0)
        max_entradas = 10;

    tRanking* rank = malloc(sizeof(tRanking));
    if (!rank)
        return NULL;

    rank->entradas = vector_crear(max_entradas, sizeof(tEntradaRanking));
    if (!rank->entradas) {
        free(rank);
        return NULL;
    }

    rank->max_entradas = max_entradas;
    return rank;
}

static bool ranking_esta_lleno(tRanking* rank)
{
    return vector_tamanio(rank->entradas) >= rank->max_entradas;
}

static tEntradaRanking* ranking_peor_entrada(tRanking* rank)
{
    size_t cant = vector_tamanio(rank->entradas);
    if (cant == 0) return NULL;

    tEntradaRanking* peor = (tEntradaRanking*)vector_obtener(rank->entradas, 0);
    for (size_t i = 1; i < cant; i++) {
        tEntradaRanking* actual = (tEntradaRanking*)vector_obtener(rank->entradas, i);
        if (actual && (!peor || actual->puntos < peor->puntos))
            peor = actual;
    }
    return peor;
}

int ranking_agregar(tRanking* rank, const char* nombre, uint32_t puntos, uint16_t lineas)
{
    if (!rank || !nombre)
        return -1;

    tEntradaRanking entrada;
    memset(&entrada, 0, sizeof(tEntradaRanking));
    strncpy(entrada.nombre, nombre, RANKING_MAX_NOMBRE - 1);
    entrada.nombre[RANKING_MAX_NOMBRE - 1] = '\0';
    entrada.puntos = puntos;
    entrada.lineas = lineas;
    formatear_fecha_hora(entrada.fecha_hora, sizeof(entrada.fecha_hora));

    if (ranking_esta_lleno(rank)) {
        tEntradaRanking* peor = ranking_peor_entrada(rank);
        if (peor && puntos <= peor->puntos)
            return -1;

        size_t indice_peor = (size_t)(peor - (tEntradaRanking*)rank->entradas->datos);
        vector_modificar(rank->entradas, indice_peor, &entrada, sizeof(entrada));
    } else {
        if (vector_agregar(rank->entradas, &entrada, sizeof(entrada)) != 0)
            return -1;
    }

    ranking_ordenar_por_puntos(rank);
    ranking_guardar(rank);
    return 0;
}

bool ranking_guardar(tRanking* rank)
{
    if (!rank)
        return false;

    FILE* fp = fopen(RANKING_ARCHIVO, "w");
    if (!fp)
        return false;

    size_t cant = vector_tamanio(rank->entradas);
    for (size_t i = 0; i < cant; i++) {
        tEntradaRanking* e = (tEntradaRanking*)vector_obtener(rank->entradas, i);
        if (fprintf(fp, "%s%c%lu%c%s\n",
                e->nombre, SEPARADOR,
                (unsigned long)e->puntos, SEPARADOR,
                e->fecha_hora) < 0) {
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

bool ranking_cargar(tRanking* rank)
{
    if (!rank)
        return false;

    FILE* fp = fopen(RANKING_ARCHIVO, "r");
    if (!fp)
        return false;

    char linea[256];
    char sep[2] = {SEPARADOR, '\0'};
    size_t cargadas = 0;

    while (fgets(linea, sizeof(linea), fp) && cargadas < rank->max_entradas) {
        char* token;
        char* resto = linea;

        token = strtok(resto, sep);
        if (!token) continue;
        tEntradaRanking entrada;
        memset(&entrada, 0, sizeof(tEntradaRanking));
        strncpy(entrada.nombre, token, RANKING_MAX_NOMBRE - 1);
        entrada.nombre[RANKING_MAX_NOMBRE - 1] = '\0';

        token = strtok(NULL, sep);
        if (!token) continue;
        entrada.puntos = (uint32_t)strtoul(token, NULL, 10);

        char* token_siguiente = strtok(NULL, sep);
        if (!token_siguiente) continue;

        char* token_extra = strtok(NULL, sep);
        if (token_extra) {
            entrada.lineas = (uint16_t)strtoul(token_siguiente, NULL, 10);
            strncpy(entrada.fecha_hora, token_extra, sizeof(entrada.fecha_hora) - 1);
        } else {
            strncpy(entrada.fecha_hora, token_siguiente, sizeof(entrada.fecha_hora) - 1);
        }
        entrada.fecha_hora[sizeof(entrada.fecha_hora) - 1] = '\0';

        vector_agregar(rank->entradas, &entrada, sizeof(tEntradaRanking));
        cargadas++;
    }

    fclose(fp);
    ranking_ordenar_por_puntos(rank);
    return true;
}

tEntradaRanking* ranking_obtener(tRanking* rank, size_t indice)
{
    if (!rank)
        return NULL;
    return (tEntradaRanking*)vector_obtener(rank->entradas, indice);
}

size_t ranking_tamanio(tRanking* rank)
{
    if (!rank)
        return 0;
    return vector_tamanio(rank->entradas);
}

bool ranking_borrar(tRanking* rank)
{
    if (!rank)
        return false;

    vector_vaciar(rank->entradas);
    remove(RANKING_ARCHIVO);
    return true;
}

static int comparar_puntos_desc(const void* a, const void* b)
{
    const tEntradaRanking* ea = (const tEntradaRanking*)a;
    const tEntradaRanking* eb = (const tEntradaRanking*)b;
    return (eb->puntos > ea->puntos) - (eb->puntos < ea->puntos);
}

void ranking_ordenar_por_puntos(tRanking* rank)
{
    if (!rank)
        return;

    size_t cant = vector_tamanio(rank->entradas);
    if (cant <= 1)
        return;

    qsort(vector_obtener(rank->entradas, 0), cant, sizeof(tEntradaRanking), comparar_puntos_desc);
}

void ranking_destruir(tRanking* rank)
{
    if (!rank)
        return;

    if (rank->entradas)
        vector_destruir(rank->entradas);

    free(rank);
}
