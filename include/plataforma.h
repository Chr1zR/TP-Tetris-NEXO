#ifndef PLATAFORMA_H_INCLUDED
#define PLATAFORMA_H_INCLUDED

#include<stdio.h>
#include<GBT/gbt.h>
#include "config.h"

int iniciar_plataforma(char* nombreVentana, size_t tam, tConfigPantalla* config);
void parar_plataforma(tGBT_Temporizador* temporizador, tGBT_Temporizador* temporizador_s);
#endif // PLATAFORMA_H_INCLUDED
