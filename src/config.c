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

    config->ventana.ancho = 640;
    config->ventana.alto = 480;
    config->ventana.escala = 1;

    config->tablero.filas = 20;  

    config->tablero.columnas = 10;   //Columnas en modalidad deluxe tienen rango [8;16]

    config->tablero.tam_bloque = (config->ventana.alto - 6) / config->tablero.filas;
    config->tablero.alto_px = config->tablero.filas * config->tablero.tam_bloque;

    
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

    config->ventana.ancho = 640;
    config->ventana.alto = 480;
    config->ventana.escala = atoi(argv[2]);

    config->tablero.filas = 20;

    modal = modalidadObtener();
    if(modal == 2){
        config->tablero.columnas = columnasDeluxeObtener();
    }
    else{
        config->tablero.columnas = 10;
    }
    config->tablero.tam_bloque = (config->ventana.alto - 6) / config->tablero.filas;
    config->tablero.alto_px = config->tablero.filas * config->tablero.tam_bloque;

    anchosCalcular(config);

    return config;
    }
    if(strcmp(argv[1],"CGA") == 0){


    config->ventana.ancho = 320;
    config->ventana.alto = 200;
    config->ventana.escala = atoi(argv[2]);

    config->tablero.filas = 20;


    modal = modalidadObtener();
    if(modal == 2){
        config->tablero.columnas = columnasDeluxeObtener();
    }
    else{
        config->tablero.columnas = 10;
    }
    config->tablero.tam_bloque = (config->ventana.alto - 6) / config->tablero.filas;
    config->tablero.alto_px = config->tablero.filas * config->tablero.tam_bloque;

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
    config->tablero.ancho_px = config->tablero.columnas * config->tablero.tam_bloque;

    // Centra tablero horizontalmente
    config->tablero.offset_x = (config->ventana.ancho - config->tablero.ancho_px) / 2;

    // Centra tablero verticalmente
    config->tablero.offset_y = (config->ventana.alto - config->tablero.alto_px) / 2;

    // Distribuye espacio a los costados
    config->hud.ancho_izq = config->tablero.offset_x;
    config->hud.ancho_der = config->ventana.ancho - config->tablero.offset_x - config->tablero.ancho_px;

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
