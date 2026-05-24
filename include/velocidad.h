#ifndef VELOCIDAD_H_INCLUDED
#define VELOCIDAD_H_INCLUDED

#include <GBT/gbt.h>
#include "logica.h"

#define INTERVALO_INICIAL 1
#define FACTOR_REDICCION 0.97
#define PIEZAS_POR_NIVEL 10

void velocidadAplicarSiCorresponde(tEstadoJuego* estado, double* intervalo_actual, tGBT_Temporizador** temporizador);

#endif
