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

    snprintf(nombreVentana, tam, "Ventana %dx%d",ANCHO_VGA_VENTANA, ALTO_VGA_VENTANA);

    if (gbt_crear_ventana(nombreVentana, ANCHO_VGA_VENTANA, ALTO_VGA_VENTANA, ESCALA_VENTANA) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        gbt_cerrar();
        return ERROR_INICIALIZACION;
    }

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0) {
        fprintf(stderr, "Error al aplicar la nueva paleta de colores: %s\n", gbt_obtener_log());
        gbt_destruir_ventana();
        gbt_cerrar();
    return -1;
    }
    return TODO_OK;
}
void parar_plataforma(tGBT_Temporizador* temporizador){
    if(temporizador){
        gbt_temporizador_destruir(temporizador);
    }
    gbt_destruir_ventana();
    gbt_cerrar();
}
