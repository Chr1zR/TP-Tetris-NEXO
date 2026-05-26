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
}tTipoTetromino;

typedef struct{
    tTipoTetromino tipo;
    uint8_t matriz[4][4];
    uint8_t rotacion;
    uint16_t x,y;
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


tTetromino* tetrominoCrear();
void tetrominoAleatorio(tTetromino* t, uint8_t col);
void tetrominoCopiar(tTetromino* destino, tTetromino* origen);
void tetrominoRotar(tTetromino* tetro, bool sentido_derecha);
void tetrominoBajar(tTetromino* tetro);
const uint8_t* tetrominoObtenerMatrizRotacion(tTipoTetromino tipo, uint8_t rotacion);
void tTetromino_Destruir(tTetromino* t);
void tetrominoCalcularLimites(tTetromino* t);
#endif // PIEZAS_H_INCLUDED
