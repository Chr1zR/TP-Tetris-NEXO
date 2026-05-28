#include "../include/errores.h"

/**
 * @desc Convierte un codigo de error a texto descriptivo.
 * @param error ErrorCodigo Codigo de error.
 * @return const char* Mensaje descriptivo.
 */
const char* error_a_texto(ErrorCodigo error){

    switch(error){

        case ERROR_INICIALIZACION:
            return "Error al inicializar libreria externa GBT";

        default:
            return "Error desconocido";
    }


}
