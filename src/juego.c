#include "../include/juego.h"
#include "../include/errores.h"
#include "../include/plataforma.h"
#include <GBT/gbt.h>
#include <time.h>
int iniciar_juego(){

    char nombreVentana[128];
    if(!iniciar_plataforma(nombreVentana, sizeof(nombreVentana))){

        return ERROR_INICIALIZACION;
    }
    tGBT_Temporizador* temporizador = gbt_temporizador_crear(1.0);
    if (!temporizador) {
        fprintf(stderr, "Error al crear el temporizador para los dibujos: %s\n", gbt_obtener_log());
        return ERROR_INICIALIZACION;
    }

    srand(time(0));
    uint8_t corriendo = 1;
    eGBT_Tecla tecla;
    unsigned int contador = 0;
    while(corriendo){

        gbt_procesar_entrada();
        tecla = gbt_obtener_tecla_presionada();
        if (tecla == GBTK_ESCAPE) {

            corriendo = 0;
            printf("Saliendo del ejemplo\n");
        }
        if (gbt_temporizador_consumir(temporizador)) {
            printf("%d\n",contador);
            contador++;
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }
    gbt_temporizador_destruir(temporizador);
    gbt_destruir_ventana();
    gbt_cerrar();
    return TODO_OK;
}
