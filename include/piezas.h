#ifndef PIEZAS_H_INCLUDED
#define PIEZAS_H_INCLUDED
#include <stdint.h>
#include <stdbool.h>
#include "../include/config.h"  //TAM_BLOQUE
#include "../include/paleta.h"  //define colores

typedef enum {

    TETROMINO_NULO = -1,
    TETROMINO_I,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_O,
    TETROMINO_S,
    TETROMINO_T,
    TETROMINO_Z,
    TETROMINO_X,
    TETROMINO_C,
    TETROMINO_P,
    TETROMINO_ESTRELLA,
}tTipoTetromino;

typedef struct sTetromino{
    tTipoTetromino tipo;
    uint8_t matriz[4][4];
    uint8_t rotacion;
    int x;
    uint16_t y;
    uint8_t min_fila, max_fila, min_col, max_col;
}tTetromino;

static const uint8_t tetromino_I_rotaciones[4][4][4] = {
    {
        {C, C, C, C},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {C, 0, 0, 0},
        {C, 0, 0, 0},
        {C, 0, 0, 0},
        {C, 0, 0, 0}
    },
    {
        {C, C, C, C},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {C, 0, 0, 0},
        {C, 0, 0, 0},
        {C, 0, 0, 0},
        {C, 0, 0, 0}
    }
};
static const uint8_t tetromino_J_rotaciones[4][4][4] = {
    {
        {A, 0, 0, 0},
        {A, A, A, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {A, A, 0, 0},
        {A, 0, 0, 0},
        {A, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {A, A, A, 0},
        {0, 0, A, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, A, 0, 0},
        {0, A, 0, 0},
        {A, A, 0, 0},
        {0, 0, 0, 0}
    }
};
static const uint8_t tetromino_L_rotaciones[4][4][4] = {
    {
        {0, 0,  NA, 0},
        {NA,NA, NA, 0},
        {0, 0,  0,  0},
        {0, 0,  0,  0}
    },
    {
        {NA,0,  0,  0},
        {NA,0,  0,  0},
        {NA,NA, 0,  0},
        {0, 0,  0,  0}
    },
    {
        {NA,NA,NA,  0},
        {NA,0,  0,  0},
        {0, 0,  0,  0},
        {0, 0,  0,  0}
    },
    {
        {NA,NA, 0,  0},
        {0, NA, 0,  0},
        {0, NA, 0,  0},
        {0, 0,  0,  0}
    }
};
static const uint8_t tetromino_O_rotaciones[4][4][4] = {
    {
        {AM,AM, 0, 0},
        {AM,AM, 0, 0},
        {0, 0,  0, 0},
        {0, 0,  0, 0}
    },
    {
        {AM,AM, 0, 0},
        {AM,AM, 0, 0},
        {0, 0,  0, 0},
        {0, 0,  0, 0}
    },
    {
        {AM,AM, 0, 0},
        {AM,AM, 0, 0},
        {0, 0,  0, 0},
        {0, 0,  0, 0}
    },
    {
        {AM,AM, 0, 0},
        {AM,AM, 0, 0},
        {0, 0,  0, 0},
        {0, 0,  0, 0}
    }
};
static const uint8_t tetromino_S_rotaciones[4][4][4] = {
    {
        {0, V, V, 0},
        {V, V, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {V, 0, 0, 0},
        {V, V, 0, 0},
        {0, V, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, V, V, 0},
        {V, V, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {V, 0, 0, 0},
        {V, V, 0, 0},
        {0, V, 0, 0},
        {0, 0, 0, 0}
    }
};
static const uint8_t tetromino_T_rotaciones[4][4][4] = {
    {
        {0, M, 0, 0},
        {M, M, M, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {M, 0, 0, 0},
        {M, M, 0, 0},
        {M, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {M, M, M, 0},
        {0, M, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, M, 0, 0},
        {M, M, 0, 0},
        {0, M, 0, 0},
        {0, 0, 0, 0}
    }
};
static const uint8_t tetromino_Z_rotaciones[4][4][4] = {
    {
        {R, R, 0, 0},
        {0, R, R, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, R, 0, 0},
        {R, R, 0, 0},
        {R, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {R, R, 0, 0},
        {0, R, R, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, R, 0, 0},
        {R, R, 0, 0},
        {R, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

static const uint8_t tetromino_X_rotaciones[4][4][4] = {
    {
        {AC, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {AC, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {AC, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {AC, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

static const uint8_t tetromino_C_rotaciones[4][4][4] = {
    {
        {NC, NC, 0, 0},
        {NC, 0, 0, 0},
        {NC, NC, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {NC, NC, NC, 0},
        {NC, 0, NC, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {NC, NC, 0, 0},
        {0, NC, 0, 0},
        {NC, NC, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {NC, 0, NC, 0},
        {NC, NC,NC, 0},
        {0, 0,  0,  0},
        {0, 0,  0,  0}
    }
};

static const uint8_t tetromino_P_rotaciones[4][4][4] = {
    {
        {AO, AO, 0, 0},
        {AO, AO, 0, 0},
        {AO, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {AO, AO, AO, 0},
        {0, AO, AO, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, AO, 0, 0},
        {AO, AO, 0, 0},
        {AO, AO, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {AO, AO, 0, 0},
        {AO, AO, AO, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

static const uint8_t tetromino_ESTRELLA_rotaciones[4][4][4] = {
    {
        {GA, GA, GA, 0},
        {0, GA, 0, 0},
        {GA, GA, GA, 0},
        {0, 0, 0, 0}
    },
    {
        {GA, 0, GA, 0},
        {GA, GA, GA, 0},
        {GA, 0, GA, 0},
        {0, 0, 0, 0}
    },
    {
        {GA, GA, GA, 0},
        {0, GA, 0, 0},
        {GA, GA, GA, 0},
        {0, 0, 0, 0}
    },
    {
        {GA, 0, GA, 0},
        {GA, GA, GA, 0},
        {GA, 0, GA, 0},
        {0, 0, 0, 0}
    }
};

/**
 * @desc Reserva memoria para un nuevo tetromino vacio.
 * @return tTetromino* Puntero al tetromino o NULL si error.
 */
tTetromino* tetrominoCrear();

/**
 * @desc Asigna tipo y posicion aleatoria a un tetromino.
 * @param t tTetromino* Puntero al tetromino.
 * @param col uint8_t Cantidad de columnas del tablero.
 * @param piezas_extras bool true para incluir piezas extra.
 * @return void
 */
void tetrominoAleatorio(tTetromino* t, uint8_t col, bool piezas_extras);

/**
 * @desc Copia los datos de un tetromino a otro.
 * @param destino tTetromino* Tetromino destino.
 * @param origen tTetromino* Tetromino origen.
 * @return void
 */
void tetrominoCopiar(tTetromino* destino, tTetromino* origen);

/**
 * @desc Rota un tetromino en el sentido indicado.
 * @param tetro tTetromino* Puntero al tetromino.
 * @param sentido_derecha bool true para derecha, false para izquierda.
 * @return void
 */
void tetrominoRotar(tTetromino* tetro, bool sentido_derecha);

/**
 * @desc Incrementa la coordenada Y del tetromino.
 * @param tetro tTetromino* Puntero al tetromino.
 * @return void
 */
void tetrominoBajar(tTetromino* tetro);

/**
 * @desc Obtiene la matriz de rotacion para un tipo y angulo.
 * @param tipo tTipoTetromino Tipo de tetromino.
 * @param rotacion uint8_t Indice de rotacion (0-3).
 * @return const uint8_t* Puntero a la matriz o NULL.
 */
const uint8_t* tetrominoObtenerMatrizRotacion(tTipoTetromino tipo, uint8_t rotacion);

/**
 * @desc Libera la memoria de un tetromino.
 * @param t tTetromino* Puntero al tetromino.
 * @return void
 */
void tTetromino_Destruir(tTetromino* t);

/**
 * @desc Calcula los limites ocupados de la matriz del tetromino.
 * @param t tTetromino* Puntero al tetromino.
 * @return void
 */
void tetrominoCalcularLimites(tTetromino* t);
#endif // PIEZAS_H_INCLUDED
