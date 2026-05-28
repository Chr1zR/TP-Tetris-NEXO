#ifndef PLATAFORMA_H_INCLUDED
#define PLATAFORMA_H_INCLUDED

#include<stdio.h>
#include<GBT/gbt.h>
#include "config.h"

/**
 * @desc Inicializa GBT, crea ventana y aplica paleta.
 * @param nombreVentana char* Buffer para el titulo.
 * @param tam size_t Tamano del buffer.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return int 0 si ok, codigo de error si falla.
 */
int iniciar_plataforma(char* nombreVentana, size_t tam, tConfigPantalla* config);

/**
 * @desc Recrea la ventana GBT con nueva resolucion.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return int 0 si ok, -1 si error.
 */
int reiniciar_plataforma(tConfigPantalla* config);

/**
 * @desc Destruye temporizadores, ventana y cierra GBT.
 * @param temporizador tGBT_Temporizador* Temporizador principal.
 * @param temporizador_s tGBT_Temporizador* Temporizador de segundos.
 * @return void
 */
void parar_plataforma(tGBT_Temporizador* temporizador, tGBT_Temporizador* temporizador_s);
#endif // PLATAFORMA_H_INCLUDED
