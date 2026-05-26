#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "paleta.h"
#include "config.h"
#include "piezas.h"

#define ALTO_NUMERO 7
#define ANCHO_NUMERO 5
#define ALTO_LETRA 5
#define ANCHO_LETRA 3

// Matrices de letras A-Z (5x3 píxeles)
static const uint8_t letra_A[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC,  0, CC},
    {CC, CC, CC},
    {CC,  0, CC},
    {CC,  0, CC}
};

static const uint8_t letra_B[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0}
};

static const uint8_t letra_C[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC,  0,  0},
    {CC,  0,  0},
    {CC,  0,  0},
    {CC, CC, CC}
};

static const uint8_t letra_D[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, 0}
};

static const uint8_t letra_E[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC, 0, 0},
    {CC, CC, 0},
    {CC, 0, 0},
    {CC, CC, CC}
};

static const uint8_t letra_F[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC, 0, 0},
    {CC, CC, 0},
    {CC, 0, 0},
    {CC, 0, 0}
};

static const uint8_t letra_G[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC,  0,  0},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC, CC}
};

static const uint8_t letra_H[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, CC},
    {CC, 0, CC},
    {CC, 0, CC}
};

static const uint8_t letra_I[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {0, CC, 0},
    {0, CC, 0},
    {0, CC, 0},
    {CC, CC, CC}
};

static const uint8_t letra_J[ALTO_LETRA][ANCHO_LETRA] = {
    { 0, CC, CC},
    { 0,  0, CC},
    { 0,  0, CC},
    { 0,  0, CC},
    {CC, CC, CC}
};

static const uint8_t letra_K[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC}
};

static const uint8_t letra_L[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, 0},
    {CC, 0, 0},
    {CC, 0, 0},
    {CC, 0, 0},
    {CC, CC, CC}
};

static const uint8_t letra_M[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, CC, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC}
};

static const uint8_t letra_N[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, CC, CC},
    {CC, CC, CC},
    {CC, 0, CC},
    {CC, 0, CC}
};

static const uint8_t letra_O[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC, CC}
};

static const uint8_t letra_P[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, 0},
    {CC, 0, 0}
};

static const uint8_t letra_Q[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC,  0}
};

static const uint8_t letra_R[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC}
};

static const uint8_t letra_S[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {CC,  0,  0},
    {CC, CC, CC},
    { 0,  0, CC},
    {CC, CC, CC}
};

static const uint8_t letra_T[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {0, CC, 0},
    {0, CC, 0},
    {0, CC, 0},
    {0, CC, 0}
};

static const uint8_t letra_U[ALTO_LETRA][ANCHO_LETRA] = {
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC,  0, CC},
    {CC, CC, CC}
};

static const uint8_t letra_V[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0},
    {0, CC, 0}
};

static const uint8_t letra_W[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, 0, CC},
    {CC, CC, CC},
    {CC, 0, CC}
};

static const uint8_t letra_X[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0},
    {CC, 0, CC},
    {CC, 0, CC}
};

static const uint8_t letra_Y[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, 0, CC},
    {CC, 0, CC},
    {0, CC, 0},
    {0, CC, 0},
    {0, CC, 0}
};

static const uint8_t letra_Z[ALTO_LETRA][ANCHO_LETRA] = {
    {CC, CC, CC},
    {0, 0, CC},
    {0, CC, 0},
    {CC, 0, 0},
    {CC, CC, CC}
};

static const uint8_t cero[ALTO_NUMERO][ANCHO_NUMERO] = {
      {0, CC, CC, CC, 0},
      {CC, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {0, CC, CC, CC, 0}
};

static const uint8_t uno[ALTO_NUMERO][ANCHO_NUMERO] = {
      { 0, CC, CC,  0,  0},
      {CC,  0, CC,  0,  0},
      { 0,  0, CC,  0,  0},
      { 0,  0, CC,  0,  0},
      { 0,  0, CC,  0,  0},
      { 0,  0, CC,  0,  0},
      {CC, CC, CC, CC, CC}
};

static const uint8_t dos[ALTO_NUMERO][ANCHO_NUMERO] = {
      {0, CC, CC, CC, 0},
      {CC, 0, 0, 0, CC},
      {0, 0, 0, 0, CC},
      {0, 0, CC, CC, 0},
      {0, CC, 0, 0, 0},
      {CC, 0, 0, 0, 0},
      {CC, CC, CC, CC, CC}
};

static const uint8_t tres[ALTO_NUMERO][ANCHO_NUMERO] = {
      {0, CC, CC, CC, 0},
      {CC, 0, 0, 0, CC},
      {0, 0, 0, 0, CC},
      {0, 0, CC, CC, 0},
      {0, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {0, CC, CC, CC, 0}
};

static const uint8_t cuatro[ALTO_NUMERO][ANCHO_NUMERO] = {
      {CC,  0,  0,  0, CC},
      {CC,  0,  0,  0, CC},
      {CC,  0,  0,  0, CC},
      {CC,  CC, CC, CC, CC},
      {0,   0,  0,  0, CC},
      {0,   0,  0,  0, CC},
      {0,   0,  0,  0, CC}
};

static const uint8_t cinco[ALTO_NUMERO][ANCHO_NUMERO] = {
      {CC, CC, CC, CC, CC},
      {CC, 0, 0, 0, 0},
      {CC, 0, 0, 0, 0},
      {0, CC, CC, CC, 0},
      {0, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {0, CC, CC, CC, 0}
};

static const uint8_t seis[ALTO_NUMERO][ANCHO_NUMERO] = {
      {0, CC, CC, CC, 0},
      {CC, 0, 0, 0, 0},
      {CC, 0, 0, 0, 0},
      {CC, CC, CC, CC, 0},
      {CC, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {0, CC, CC, CC, 0}
};

static const uint8_t siete[ALTO_NUMERO][ANCHO_NUMERO] = {
      {CC, CC, CC, CC, CC},
      {CC, 0, 0, 0, CC},
      {0, 0, 0, 0, CC},
      {0, 0, 0, CC, 0},
      {0, 0, CC, 0, 0},
      {0, CC, 0, 0, 0},
      {CC, 0, 0, 0, 0}
};

static const uint8_t ocho[ALTO_NUMERO][ANCHO_NUMERO] = {
      { 0, CC, CC, CC,  0},
      {CC,  0,  0,  0, CC},
      {CC,  0,  0,  0, CC},
      {CC, CC, CC, CC, CC},
      {CC,  0,  0,  0, CC},
      {CC,  0,  0,  0, CC},
      { 0, CC, CC, CC,  0}
};

static const uint8_t nueve[ALTO_NUMERO][ANCHO_NUMERO] = {
      {0, CC, CC, CC, 0},
      {CC, 0, 0, 0, CC},
      {CC, 0, 0, 0, CC},
      {0, CC, CC, CC, CC},
      {0, 0, 0, 0, CC},
      {0, 0, 0, 0, CC},
      {0, CC, CC, CC, 0}
};

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

#endif // HUD_H_INCLUDED
