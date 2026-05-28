#ifndef VELOCIDAD_H_INCLUDED
#define VELOCIDAD_H_INCLUDED

#include <GBT/gbt.h>
#include "logica.h"

#define INTERVALO_INICIAL 1
#define FACTOR_REDUCCION 0.97
#define PIEZAS_POR_NIVEL 10

/**
 * @desc Aumenta nivel y reduce intervalo cada N piezas.
 * @param estado tEstadoJuego* Estado actual del juego.
 * @param intervalo_actual double* Puntero al intervalo actual.
 * @param temporizador tGBT_Temporizador** Puntero al temporizador GBT.
 * @return void
 */
void velocidadAplicarSiCorresponde(tEstadoJuego* estado, double* intervalo_actual, tGBT_Temporizador** temporizador);

#endif
