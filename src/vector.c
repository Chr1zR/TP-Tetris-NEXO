#include "../include/vector.h"
#include <stdlib.h>
#include <string.h>

tVector* vector_crear(size_t capacidad_inicial, size_t tamElem)
{
    if (capacidad_inicial == 0)
        capacidad_inicial = 8;

    tVector* v = malloc(sizeof(tVector));
    if (!v)
        return NULL;

    v->datos = malloc(capacidad_inicial * tamElem);
    if (!v->datos) {
        free(v);
        return NULL;
    }

    v->cap = capacidad_inicial;
    v->ce = 0;
    v->tamElem = tamElem;

    return v;
}

int vector_agregar(tVector* v, const void* elemento, size_t tam_elemento)
{
    if (!v || !elemento)
        return -1;

    if (v->tamElem != tam_elemento)
        return -1;

    if (v->ce >= v->cap) {
        size_t nueva_cap = v->cap * 2;
        void* nuevos_datos = realloc(v->datos, nueva_cap * v->tamElem);
        if (!nuevos_datos)
            return -1;
        v->datos = nuevos_datos;
        v->cap = nueva_cap;
    }

    char* dst = (char*)v->datos + (v->ce * v->tamElem);
    memcpy(dst, elemento, tam_elemento);
    v->ce++;

    return 0;
}

void* vector_obtener(tVector* v, size_t indice)
{
    if (!v || indice >= v->ce)
        return NULL;

    return (char*)v->datos + (indice * v->tamElem);
}

int vector_modificar(tVector* v, size_t indice, const void* elemento, size_t tam_elemento)
{
    if (!v || !elemento)
        return -1;
    if (indice >= v->ce)
        return -1;
    if (v->tamElem != tam_elemento)
        return -1;

    char* dst = (char*)v->datos + (indice * v->tamElem);
    memcpy(dst, elemento, tam_elemento);
    return 0;
}

size_t vector_tamanio(tVector* v)
{
    if (!v)
        return 0;
    return v->ce;
}

size_t vector_capacidad(tVector* v)
{
    if (!v)
        return 0;
    return v->cap;
}

bool vector_vacio(tVector* v)
{
    if (!v)
        return true;
    return v->ce == 0;
}

void vector_vaciar(tVector* v)
{
    if (v)
        v->ce = 0;
}

void vector_destruir(tVector* v)
{
    if (!v)
        return;

    if (v->datos)
        free(v->datos);

    free(v);
}
