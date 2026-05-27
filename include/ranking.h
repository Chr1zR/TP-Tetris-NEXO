#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "vector.h"

#define RANKING_MAX_NOMBRE 20
#define RANKING_ARCHIVO "ranking.txt"

typedef struct {
    char nombre[RANKING_MAX_NOMBRE];
    uint32_t puntos;
    uint16_t lineas;
    char fecha_hora[20];
} tEntradaRanking;

typedef struct {
    tVector* entradas;
    size_t max_entradas;
} tRanking;

tRanking* ranking_crear(size_t max_entradas);
int ranking_agregar(tRanking* rank, const char* nombre, uint32_t puntos, uint16_t lineas);
bool ranking_guardar(tRanking* rank);
bool ranking_cargar(tRanking* rank);
tEntradaRanking* ranking_obtener(tRanking* rank, size_t indice);
size_t ranking_tamanio(tRanking* rank);
bool ranking_borrar(tRanking* rank);
void ranking_ordenar_por_puntos(tRanking* rank);
void ranking_destruir(tRanking* rank);

#endif // RANKING_H_INCLUDED
