#ifndef PALETA_H_INCLUDED
#define PALETA_H_INCLUDED

#include<GBT/gbt.h>

#define CANT_COLORES 15

#define N 0     //Negro
#define A 1     //Azul
#define V 2     //Verde
#define C 3     // Celeste/ Cian
#define R 4     //Rojo
#define M 5     //Magenta
#define AM 6    //Amarillo
#define NA 7     //Naranja
#define NC 8    //Naranja claro
#define AC 9    //Amarillo claro
#define CC 10   //Celeste claro
#define CO 11   //Celeste oscuro
#define AO 12   //Azul oscuro
#define GA 13   //Gris acero (azul-gris oscuro, fondo ladrillado)
#define T 14    //Transparente

extern tGBT_ColorRGB paletaCGA[CANT_COLORES];

#endif // PALETA_H_INCLUDED
