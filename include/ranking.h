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

/**
 * @desc Crea una estructura de ranking vacia.
 * @param max_entradas size_t Cantidad maxima de entradas.
 * @return tRanking* Puntero al ranking o NULL si error.
 */
tRanking* ranking_crear(size_t max_entradas);

/**
 * @desc Agrega una nueva entrada al ranking.
 * @param rank tRanking* Puntero al ranking.
 * @param nombre const char* Nombre del jugador.
 * @param puntos uint32_t Puntaje obtenido.
 * @param lineas uint16_t Lineas limpiadas.
 * @return int 0 si ok, -1 si no califica.
 */
int ranking_agregar(tRanking* rank, const char* nombre, uint32_t puntos, uint16_t lineas);

/**
 * @desc Guarda el ranking a archivo de texto.
 * @param rank tRanking* Puntero al ranking.
 * @return bool true si ok, false si error.
 */
bool ranking_guardar(tRanking* rank);

/**
 * @desc Carga el ranking desde archivo de texto.
 * @param rank tRanking* Puntero al ranking.
 * @return bool true si ok, false si error.
 */
bool ranking_cargar(tRanking* rank);

/**
 * @desc Obtiene una entrada del ranking por indice.
 * @param rank tRanking* Puntero al ranking.
 * @param indice size_t Posicion de la entrada.
 * @return tEntradaRanking* Puntero a la entrada o NULL.
 */
tEntradaRanking* ranking_obtener(tRanking* rank, size_t indice);

/**
 * @desc Devuelve la cantidad de entradas en el ranking.
 * @param rank tRanking* Puntero al ranking.
 * @return size_t Cantidad de entradas.
 */
size_t ranking_tamanio(tRanking* rank);

/**
 * @desc Vacia el ranking y elimina el archivo.
 * @param rank tRanking* Puntero al ranking.
 * @return bool true si ok, false si error.
 */
bool ranking_borrar(tRanking* rank);

/**
 * @desc Ordena las entradas por puntaje descendente.
 * @param rank tRanking* Puntero al ranking.
 * @return void
 */
void ranking_ordenar_por_puntos(tRanking* rank);

/**
 * @desc Libera la memoria del ranking.
 * @param rank tRanking* Puntero al ranking.
 * @return void
 */
void ranking_destruir(tRanking* rank);

#endif // RANKING_H_INCLUDED
