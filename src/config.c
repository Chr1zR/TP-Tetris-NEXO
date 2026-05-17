#include "../include/config.h"
#include "../include/hud.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
tConfigPantalla* crear_config_default(){

    tConfigPantalla* config = calloc(1,sizeof(tConfigPantalla));
    if(!config){
        return NULL;
    }
    config->ancho_ventana = 640;
    config->alto_ventana = 480;
    config->escala = 1;

    config->filas = 20;
    config->columnas = 10;      //Columnas en modalidad deluxe tienen rango [8;16]

    config->tam_bloque = (config->alto_ventana - 6) / config->filas;
    config->alto_tablero = config->filas * config->tam_bloque;

    anchosCalcular(config);

    return config;
}
tConfigPantalla* crear_config(char* argv[]){

    if(!argv){
        return NULL;
    }
    uint8_t modal = 0;

    tConfigPantalla* config = calloc(1,sizeof(tConfigPantalla));
    if(!config){
        return NULL;
    }

    if(strcmp(argv[1],"VGA") == 0){

    config->ancho_ventana = 640;
    config->alto_ventana = 480;
    config->escala = atoi(argv[2]);

    config->filas = 20;

    modal = modalidadObtener();
    if(modal == 2){
        config->columnas = columnasDeluxeObtener();
    }
    else{
        config->columnas = 10;
    }
    config->tam_bloque = (config->alto_ventana - 6) / config->filas;
    config->alto_tablero = config->filas * config->tam_bloque;

    anchosCalcular(config);

    return config;
    }
    if(strcmp(argv[1],"CGA") == 0){

    config->ancho_ventana = 320;
    config->alto_ventana = 200;
    config->escala = atoi(argv[2]);

    config->filas = 20;

    modal = modalidadObtener();
    if(modal == 2){
        config->columnas = columnasDeluxeObtener();
    }
    else{
        config->columnas = 10;
    }
    config->tam_bloque = (config->alto_ventana - 6) / config->filas;
    config->alto_tablero = config->filas * config->tam_bloque;

    anchosCalcular(config);

    return config;
    }
    free(config);
    config=NULL;
    return NULL;
}
void configDestruir(tConfigPantalla* config){

    if(config){
        free(config);
    }
}
void anchosCalcular(tConfigPantalla* config){
    if(!config){
        return;
    }
    // Calcula ancho_tablero basado en columnas
    config->ancho_tablero = config->columnas * config->tam_bloque;

    // Centra tablero horizontalmente
    config->offset_tablero_x = (config->ancho_ventana - config->ancho_tablero) / 2;

    // Centra tablero verticalmente
    config->offset_tablero_y = (config->alto_ventana - config->alto_tablero) / 2;

    // Distribuye espacio a los costados
    config->ancho_hud_izq = config->offset_tablero_x;
    config->ancho_hud_der = config->ancho_ventana - config->offset_tablero_x - config->ancho_tablero;

    disposicionHudCalcular(config);
}

uint8_t modalidadObtener(){

    uint8_t opcion = 0;
    while(opcion != 1 && opcion != 2) {
        printf("Seleccionar modalidad:\n");
        printf("1 - CLASSIC\n");
        printf("2 - DELUXE\n");
        printf("Opcion: ");
        scanf("%hhu", &opcion);

        if(opcion != 1 && opcion != 2) {

            printf("Opcion invalida. Intenta de nuevo.\n\n");
        }
      }
    return opcion;
}
uint8_t columnasDeluxeObtener(){

    uint8_t columnas = 0;

    while(columnas < 8 || columnas > 16) {
        printf("Ingresa columnas (8-16): ");
        scanf("%hhu", &columnas);

        if(columnas < 8 || columnas > 16) {
            printf("Rango inválido (8-16). Intenta de nuevo.\n");
        }
    }

    return columnas;
}
