#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void* datos;
    size_t cap;
    size_t ce;
    size_t tamElem;
} tVector;

/**
 * @desc Crea un vector dinamico vacio.
 * @param capacidad_inicial size_t Capacidad inicial del vector.
 * @param tamElem size_t Tamano de cada elemento.
 * @return tVector* Puntero al vector o NULL si error.
 */
tVector* vector_crear(size_t capacidad_inicial, size_t tamElem);

/**
 * @desc Agrega un elemento al final del vector.
 * @param v tVector* Puntero al vector.
 * @param elemento const void* Puntero al dato a copiar.
 * @param tam_elemento size_t Tamano del elemento.
 * @return int 0 si ok, -1 si error.
 */
int vector_agregar(tVector* v, const void* elemento, size_t tam_elemento);

/**
 * @desc Obtiene un puntero al elemento en el indice dado.
 * @param v tVector* Puntero al vector.
 * @param indice size_t Posicion del elemento.
 * @return void* Puntero al elemento o NULL si fuera de rango.
 */
void* vector_obtener(tVector* v, size_t indice);

/**
 * @desc Modifica el valor de un elemento existente.
 * @param v tVector* Puntero al vector.
 * @param indice size_t Posicion a modificar.
 * @param elemento const void* Nuevo valor.
 * @param tam_elemento size_t Tamano del elemento.
 * @return int 0 si ok, -1 si error.
 */
int vector_modificar(tVector* v, size_t indice, const void* elemento, size_t tam_elemento);

/**
 * @desc Devuelve la cantidad de elementos actuales.
 * @param v tVector* Puntero al vector.
 * @return size_t Cantidad de elementos.
 */
size_t vector_tamanio(tVector* v);

/**
 * @desc Devuelve la capacidad total reservada.
 * @param v tVector* Puntero al vector.
 * @return size_t Capacidad maxima.
 */
size_t vector_capacidad(tVector* v);

/**
 * @desc Verifica si el vector esta vacio.
 * @param v tVector* Puntero al vector.
 * @return bool true si vacio, false si no.
 */
bool vector_vacio(tVector* v);

/**
 * @desc Resetea la cantidad de elementos a cero.
 * @param v tVector* Puntero al vector.
 * @return void
 */
void vector_vaciar(tVector* v);

/**
 * @desc Libera la memoria del vector y sus datos.
 * @param v tVector* Puntero al vector.
 * @return void
 */
void vector_destruir(tVector* v);

#endif // VECTOR_H_INCLUDED
