#include "../include/vector.h"
#include <stdlib.h>
#include <string.h>

struct sVector {
    void* datos;
    size_t capacidad;
    size_t cantidad;
    size_t element_size;
};

tVector* vector_crear(size_t capacidad_inicial)
{
    if (capacidad_inicial == 0)
        capacidad_inicial = 8;

    tVector* v = malloc(sizeof(tVector));
    if (!v)
        return NULL;

    v->datos = malloc(capacidad_inicial * 64);
    if (!v->datos) {
        free(v);
        return NULL;
    }

    v->capacidad = capacidad_inicial;
    v->cantidad = 0;
    v->element_size = 0;

    return v;
}

int vector_agregar(tVector* v, const void* elemento, size_t tam_elemento)
{
    if (!v || !elemento)
        return -1;

    if (v->element_size == 0)
        v->element_size = tam_elemento;

    if (v->element_size != tam_elemento)
        return -1;

    if (v->cantidad >= v->capacidad) {
        size_t nueva_cap = v->capacidad * 2;
        void* nuevos_datos = realloc(v->datos, nueva_cap * v->element_size);
        if (!nuevos_datos)
            return -1;
        v->datos = nuevos_datos;
        v->capacidad = nueva_cap;
    }

    char* dst = (char*)v->datos + (v->cantidad * v->element_size);
    memcpy(dst, elemento, tam_elemento);
    v->cantidad++;

    return 0;
}

void* vector_obtener(tVector* v, size_t indice)
{
    if (!v || indice >= v->cantidad)
        return NULL;

    return (char*)v->datos + (indice * v->element_size);
}

size_t vector_tamanio(tVector* v)
{
    if (!v)
        return 0;
    return v->cantidad;
}

size_t vector_capacidad(tVector* v)
{
    if (!v)
        return 0;
    return v->capacidad;
}

bool vector_vacio(tVector* v)
{
    if (!v)
        return true;
    return v->cantidad == 0;
}

void vector_vaciar(tVector* v)
{
    if (v)
        v->cantidad = 0;
}

void vector_destruir(tVector* v)
{
    if (!v)
        return;

    if (v->datos)
        free(v->datos);

    free(v);
}