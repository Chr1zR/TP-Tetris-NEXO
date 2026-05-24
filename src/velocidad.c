#include "../include/velocidad.h"

void velocidadAplicarSiCorresponde(tEstadoJuego* estado, double* intervalo_actual, tGBT_Temporizador** temporizador)
{
    if(!estado || !intervalo_actual || !temporizador)
        return;

    if(estado->piezas_totales > 0 && estado->piezas_totales % PIEZAS_POR_NIVEL == 0){
        *intervalo_actual *= FACTOR_REDICCION;

        if(*temporizador){
            gbt_temporizador_destruir(*temporizador);
        }
        *temporizador = gbt_temporizador_crear(*intervalo_actual);
    }
}
