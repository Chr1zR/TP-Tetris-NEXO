/*
Apellido: Roncati, Christian Luis
DNI: 41332491
Usuario: Chr1z
Entrega: Si

Apellido: Busolin, Agustin Osvaldo
DNI: 44792752
Usuario: agusbuso
Entrega: Si

Apellido: Vega, Marcos Julian Aguero
DNI: -
Usuario: OsoMark
Entrega: No	-> Abandono cursada


*/

#include <stdio.h>
#include "../include/juego.h"

/**
 * @desc Punto de entrada del programa.
 * @param argc int Cantidad de argumentos.
 * @param argv char** Vector de argumentos.
 * @return int Codigo de salida.
 */
int main(int argc, char* argv[]){

	if(!iniciar_juego(argc, argv)){
        return -1;
	}
	return 0;
}
