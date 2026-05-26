#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct sVector tVector;

tVector* vector_crear(size_t capacidad_inicial);
int vector_agregar(tVector* v, const void* elemento, size_t tam_elemento);
void* vector_obtener(tVector* v, size_t indice);
size_t vector_tamanio(tVector* v);
size_t vector_capacidad(tVector* v);
bool vector_vacio(tVector* v);
void vector_vaciar(tVector* v);
void vector_destruir(tVector* v);

#endif // VECTOR_H_INCLUDED