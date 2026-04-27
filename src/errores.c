#include "../include/errores.h"

const char* error_a_texto(ErrorCodigo error){

    switch(error){

        case ERROR_INICIALIZACION:
            return "Error al inicializar libreria externa GBT";

        default:
            return "Error desconocido";
    }


}
