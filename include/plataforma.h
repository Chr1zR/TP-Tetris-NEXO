#ifndef PLATAFORMA_H_INCLUDED
#define PLATAFORMA_H_INCLUDED

#include<stdio.h>
#include<GBT/gbt.h>

int iniciar_plataforma(char* nombreVentana, size_t tam);
void parar_plataforma(tGBT_Temporizador* temporizador);
#endif // PLATAFORMA_H_INCLUDED
