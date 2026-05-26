#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "paleta.h"
#include "config.h"
#include "piezas.h"
#include "ranking.h"

#define PRESENTACION_CANT_OPCIONES 6

#define OPCIONES_CANT_OPCIONES 5

#define ALTO_LETRA 5
#define ALTO_NUMERO 7

// Matrices de letras A-Z con anchos reales en pixeles
// A - 5x3
static const uint8_t letra_A[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC,  0, CC},
    {CC, CC, CC},
    {CC,  0, CC},
    {CC,  0, CC}
};

// B - 5x3
static const uint8_t letra_B[ALTO_LETRA][3] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0}
};

// C - 5x3
static const uint8_t letra_C[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC,  0,  0},
    {CC,  0,  0},
    {CC,  0,  0},
    {CC, CC, CC}
};

// D - 5x3
static const uint8_t letra_D[ALTO_LETRA][3] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, 0}
};

// E - 5x3
static const uint8_t letra_E[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC, 0, 0},
    {CC, CC, 0},
    {CC, 0, 0},
    {CC, CC, CC}
};

// F - 5x3
static const uint8_t letra_F[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC, 0, 0},
    {CC, CC, 0},
    {CC, 0, 0},
    {CC, 0, 0}
};

// G - 5x3
static const uint8_t letra_G[ALTO_LETRA][4] = {
    {CC, CC, CC,CC},
    {CC,  0,  0,0},
    {CC,  0, CC,CC},
    {CC,  0, 0, CC},
    {CC, CC, CC,CC}
};

// H - 5x3
static const uint8_t letra_H[ALTO_LETRA][3] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, CC},
    {CC, 0, CC},
    {CC, 0, CC}
};

// I - 5x1
static const uint8_t letra_I[ALTO_LETRA][3] = {
    {CC,CC, CC},
    {0, CC, 0},
    {0, CC, 0},
    {0, CC, 0},
    {CC,CC, CC}
};

// J - 5x2
static const uint8_t letra_J[ALTO_LETRA][3] = {
    {0, 0,  CC},
    {0, 0,  CC},
    {0, 0,  CC},
    {CC,0,  CC},
    {CC,CC, CC}
};

// K - 5x2
static const uint8_t letra_K[ALTO_LETRA][4] = {
    {CC, 0, 0, CC},
    {CC, 0, CC, 0},
    {CC, CC,0,  0},
    {CC, 0, CC, 0},
    {CC, 0, 0, CC}
};

// L - 5x2
static const uint8_t letra_L[ALTO_LETRA][2] = {
    {CC, 0},
    {CC, 0},
    {CC, 0},
    {CC, 0},
    {CC, CC}
};

// M - 5x3
static const uint8_t letra_M[ALTO_LETRA][5] = {
    {CC, 0, 0,  0,  CC},
    {CC, CC,0,  CC, CC},
    {CC, 0, CC, 0,  CC},
    {CC, 0, 0,  0,  CC},
    {CC, 0, 0,  0,  CC}
};

// N - 5x3
static const uint8_t letra_N[ALTO_LETRA][4] = {
    {CC, 0, 0,  CC},
    {CC, CC, 0, CC},
    {CC, 0, CC, CC},
    {CC, 0, 0,  CC},
    {CC, 0, 0,  CC}
};

// O - 5x3
static const uint8_t letra_O[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC, CC}
};

// P - 5x3
static const uint8_t letra_P[ALTO_LETRA][3] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, 0},
    {CC, 0, 0}
};

// Q - 5x3
static const uint8_t letra_Q[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC,  0}
};

// R - 5x3
static const uint8_t letra_R[ALTO_LETRA][3] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC}
};

// S - 5x3
static const uint8_t letra_S[ALTO_LETRA][3] = {
    {CC, CC, CC},
    {CC,  0,  0},
    {CC, CC, CC},
    { 0,  0, CC},
    {CC, CC, CC}
};

// T - 5x1
static const uint8_t letra_T[ALTO_LETRA][3] = {
    {CC,CC,CC},
    {0, CC,0},
    {0, CC,0},
    {0, CC,0},
    {0, CC,0}
};

// U - 5x3
static const uint8_t letra_U[ALTO_LETRA][3] = {
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC, CC}
};

// V - 5x2
static const uint8_t letra_V[ALTO_LETRA][3] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0}
};

// W - 5x3
static const uint8_t letra_W[ALTO_LETRA][5] = {
    {CC, 0, 0, 0,   CC},
    {CC, 0, 0, 0,   CC},
    {CC, 0, CC, 0,  CC},
    {CC, CC, 0,CC,  CC},
    {CC, 0, 0, 0,   CC}
};

// X - 5x2
static const uint8_t letra_X[ALTO_LETRA][5] = {
    {CC, 0, 0,  0,  CC},
    {0, CC, 0,  CC,  0},
    {0, 0, CC,  0,  0},
    {0, CC, 0,  CC,  0},
    {CC, 0, 0,  0,  CC}
};

// Y - 5x2
static const uint8_t letra_Y[ALTO_LETRA][5] = {
    {CC, 0, 0,  0,  CC},
    {0, CC, 0,  CC,  0},
    {0, 0, CC,  0,  0},
    {0, 0, CC,  0,  0},
    {0, 0, CC,  0,  0}
};

// Z - 5x3
static const uint8_t letra_Z[ALTO_LETRA][4] = {
    {CC, CC, CC, CC},
    {0, 0, 0,  CC},
    {0, 0, CC,  0},
    {0, CC, 0,  0},
    {CC, CC, CC,CC}
};

// Anchos de letras A-Z (en pixeles reales)
static const uint8_t ancho_letras[26] = {
    3, 3, 3, 3, 3, 3, 4, 3, 3, 3,  // A-J
    4, 2, 5, 4, 3, 3, 3, 3, 3, 3,  // K-T
    3, 3, 5, 5, 5, 4               // U-Z
};

// Matrices de digitos 0-9 con anchos reales
// 0 - 7x3
static const uint8_t cero[ALTO_NUMERO][3] = {
    {0, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0}
};

// 1 - 7x2
static const uint8_t uno[ALTO_NUMERO][4] = {
    {0, 0, CC,  0},
    {0, CC,CC,  0},
    {CC, 0, CC,  0},
    {0, 0, CC,  0},
    {0, 0, CC,  0},
    {0, 0, CC,  0},
    {CC, CC,CC, CC}
};

// 2 - 7x3
static const uint8_t dos[ALTO_NUMERO][3] = {
    {0, CC, 0},
    {CC, 0, CC},
    {0, 0, CC},
    {0, CC, 0},
    {0, CC, 0},
    {CC, 0, 0},
    {CC, CC, CC}
};

// 3 - 7x3
static const uint8_t tres[ALTO_NUMERO][3] = {
    {0, CC, 0},
    {CC, 0, CC},
    {0, 0, CC},
    {0, CC, 0},
    {0, 0, CC},
    {CC, 0, CC},
    {0, CC, 0}
};

// 4 - 7x3
static const uint8_t cuatro[ALTO_NUMERO][3] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, CC},
    {0, 0, CC},
    {0, 0, CC},
    {0, 0, CC}
};

// 5 - 7x3
static const uint8_t cinco[ALTO_NUMERO][3] = {
    {CC, CC, CC},
    {CC, 0, 0},
    {CC, 0, 0},
    {0, CC, 0},
    {0, 0,  CC},
    {CC, 0, CC},
    {0, CC, 0}
};

// 6 - 7x3
static const uint8_t seis[ALTO_NUMERO][3] = {
    {0, CC, CC},
    {CC, 0, 0},
    {CC, 0, 0},
    {0, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0}
};

// 7 - 7x3
static const uint8_t siete[ALTO_NUMERO][3] = {
    {CC, CC, 0},
    {0, CC, 0},
    {0, CC, 0},
    {CC, CC, CC},
    {0, CC, 0},
    {0, CC, 0},
    {0, CC, 0}
};

// 8 - 7x3
static const uint8_t ocho[ALTO_NUMERO][3] = {
    {0, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0}
};

// 9 - 7x3
static const uint8_t nueve[ALTO_NUMERO][3] = {
    {0, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, CC},
    {0, 0, CC},
    {0, 0, CC},
    {CC, CC, 0}
};

// Anchos de digitos 0-9 (en pixeles reales)
static const uint8_t ancho_digitos[10] = {
    3, 4, 3, 3, 3, 3, 3, 3, 3, 3
};

// Consulta ancho de un caracter (lookup O(1))
uint8_t hudCaracterAncho(char c);

void disposicionHudCalcular(tConfigPantalla* config);

void hudRecuadrosDibujar(tConfigPantalla* config);
void hudPalabraDibujar(const char* palabra, uint16_t pos_x, uint16_t pos_y, uint8_t color, uint8_t escala, tConfigPantalla* config);
void hudScoreDibujar(uint32_t score, uint8_t escala, tConfigPantalla* config);
void hudNivelDibujar(uint8_t nivel, uint8_t escala, tConfigPantalla* config);
void hudLineasDibujar(uint16_t lineas, uint8_t escala, tConfigPantalla* config);
void hudTiempoDibujar(uint16_t tiempo_segundos, uint8_t escala, tConfigPantalla* config);
void hudNextDibujar(tTetromino* siguiente, tConfigPantalla* config);
void hudPiezasDibujar(uint16_t conteo[7], tConfigPantalla* config);
void hudTituloNeonDibujar(tConfigPantalla* config);
void cubrirPausaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada);
void cubrirGameOverDibujar(tConfigPantalla* config, uint32_t score, uint16_t lineas, const char* nombre, uint8_t opcion_seleccionada);
void cubrirConfirmarSalidaDibujar(tConfigPantalla* config);
void presentarPantallaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada);
void menuConfiguracionDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada, uint8_t modalidad, uint8_t columnas_dx, double velocidad, uint8_t confirmacion_guardado);
void pantallaAcercaDeDibujar(tConfigPantalla* config);
void pantallaCargarPartidaDibujar(tConfigPantalla* config);
void pantallaPuntajesDibujar(tConfigPantalla* config, tRanking* rank, uint8_t confirmar_borrar);
void pantallaIngresarNombreDibujar(tConfigPantalla* config, const char* buffer, uint8_t cursor_pos, bool cursor_visible);
void pantallaMensajeDibujar(tConfigPantalla* config, const char* titulo, const char* mensaje);

#endif // HUD_H_INCLUDED