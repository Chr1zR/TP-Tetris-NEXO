#include "../include/plataforma.h"

#include<stdio.h>
#include<GBT/gbt.h>
#include "../include/config.h"
#include "../include/errores.h"
#include "../include/paleta.h"
int iniciar_plataforma(char* nombreVentana, size_t tam){

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return ERROR_INICIALIZACION;
    }

    snprintf(nombreVentana, tam, "Ventana %dx%d",ANCHO_VENTANA, ALTO_VENTANA);

    if (gbt_crear_ventana(nombreVentana, ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return ERROR_INICIALIZACION;
    }

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0) {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
    return -1;
    }
    return TODO_OK;
}
