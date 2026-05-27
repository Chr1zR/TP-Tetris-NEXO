#include "../include/plataforma.h"

#include<stdio.h>
#include<GBT/gbt.h>
#include "../include/config.h"
#include "../include/errores.h"
#include "../include/paleta.h"

int iniciar_plataforma(char* nombreVentana, size_t tam, tConfigPantalla* config){

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return ERROR_INICIALIZACION;
    }

    snprintf(nombreVentana, tam, "Ventana %dx%d",config->ventana.ancho, config->ventana.alto);

    if (gbt_crear_ventana(nombreVentana, config->ventana.ancho, config->ventana.alto, config->ventana.escala) != 0) {
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
int reiniciar_plataforma(tConfigPantalla* config){
    if(!config)
        return -1;

    gbt_destruir_ventana();

    char nombreVentana[128];
    snprintf(nombreVentana, sizeof(nombreVentana), "Ventana %dx%d", config->ventana.ancho, config->ventana.alto);

    if (gbt_crear_ventana(nombreVentana, config->ventana.ancho, config->ventana.alto, config->ventana.escala) != 0) {
        fprintf(stderr, "Error al recrear ventana de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    if (gbt_aplicar_paleta(paletaCGA, CANT_COLORES, GBT_FORMATO_888) != 0) {
        fprintf(stderr, "Error al aplicar paleta: %s\n", gbt_obtener_log());
        gbt_destruir_ventana();
        return -1;
    }

    return 0;
}

void parar_plataforma(tGBT_Temporizador* temporizador, tGBT_Temporizador* temporizador_s){
    if(temporizador){
        gbt_temporizador_destruir(temporizador);
        temporizador = NULL;
    }
    if(temporizador_s){
        gbt_temporizador_destruir(temporizador_s);
        temporizador_s = NULL;
    }
    gbt_destruir_ventana();
    gbt_cerrar();
}
