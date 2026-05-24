#include "../include/errores.h"
#include "../include/logica.h"
#include "../include/config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> //printf

///funciones tTablero
tTablero* crear_tablero(tConfigPantalla* config)
{
    if(!config || config->filas <= 0 || config->columnas <= 0)
    {
        return NULL;
    }
    tTablero* tablero = malloc(sizeof(tTablero));

    if(!tablero)
    {
        return NULL;
    }
    tablero->tablero = calloc(config->filas, sizeof(unsigned char *));
    if(!(tablero -> tablero))
    {
        free(tablero);
        return NULL;
    }
    for(size_t i = 0; i < config->filas; i++)
    {
        tablero->tablero[i] = calloc(config->columnas, sizeof(unsigned char));
        if(!tablero->tablero [i])
        {
            for(size_t j = 0; j < i ; j++)
            {
                free(tablero->tablero[j]);
            }
            free(tablero->tablero);
            free(tablero);
            return NULL;
        }
    }

    tablero->pieza_actual = tetrominoCrear();
    if(!tablero->pieza_actual)
    {
        vaciar_tablero(tablero, config);
        tablero = NULL;
        return NULL;
    }
    tablero->pieza_siguiente = tetrominoCrear();
    if(!tablero->pieza_siguiente)
    {
        vaciar_tablero(tablero, config);
        tablero = NULL;
        return NULL;
    }
    return tablero;
}
void vaciar_tablero(tTablero* tablero, tConfigPantalla* config)
{
    if(!tablero)
    {
        return;
    }
    if(tablero->tablero)
    {
        for(uint8_t i = 0; i < config->filas; i++)
        {
            free(tablero->tablero[i]);
        }
        free(tablero->tablero);
        tablero->tablero = NULL;
    }
    if(tablero->pieza_actual)
    {
        tTetromino_Destruir(tablero->pieza_actual);
        tablero->pieza_actual = NULL;
    }
    if(tablero->pieza_siguiente)
    {
        tTetromino_Destruir(tablero->pieza_siguiente);
        tablero->pieza_siguiente = NULL;
    }
    free(tablero);
}
///Funciones tEstadoJuego
tEstadoJuego* tEstadoCrear(){
    tEstadoJuego* estado = (tEstadoJuego*)malloc(sizeof(tEstadoJuego));
    
    if (estado == NULL) {
        return NULL;
    }

    estado->puntos = 0;
    estado->lineas_limpias = 0;
    estado->nivel = 0;

    memset(estado->conteo_piezas, 0, sizeof(estado->conteo_piezas));

    return estado;
}
void tEstadoDestruir(tEstadoJuego* estado)
{
    if (estado != NULL) {
        free(estado);
    }
}

///Dibuja solo las esquinas de un rectangulo neon (marco incompleto estilo imagen de referencia)
static void neonEsquinas(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                          uint8_t color, uint16_t clen, uint16_t sw, uint16_t sh)
{
    for (uint16_t i = 0; i < clen; i++) {
        // Trazos horizontales desde cada esquina
        if (x0 + i < sw)                    gbt_dibujar_pixel(x0 + i, y0, color);
        if (x1 >= i && x1 - i < sw)         gbt_dibujar_pixel(x1 - i, y0, color);
        if (x0 + i < sw && y1 < sh)         gbt_dibujar_pixel(x0 + i, y1, color);
        if (x1 >= i && x1 - i < sw && y1 < sh) gbt_dibujar_pixel(x1 - i, y1, color);
        // Trazos verticales desde cada esquina
        if (y0 + i < sh)                    gbt_dibujar_pixel(x0, y0 + i, color);
        if (y1 >= i && y1 - i < sh)         gbt_dibujar_pixel(x0, y1 - i, color);
        if (y0 + i < sh && x1 < sw)         gbt_dibujar_pixel(x1, y0 + i, color);
        if (y1 >= i && y1 - i < sh && x1 < sw) gbt_dibujar_pixel(x1, y1 - i, color);
    }
}

///Fondo temático Tron Legacy - ladrillado con marcos neon decorativos
void fondoTronDibujar(tConfigPantalla* config)
{
    if (!config) return;

    uint16_t tx = config->offset_tablero_x;
    uint16_t ty = config->offset_tablero_y;
    uint16_t tw = config->ancho_tablero;
    uint16_t th = config->alto_tablero;
    uint16_t sw = config->ancho_ventana;
    uint16_t sh = config->alto_ventana;
    uint16_t rx = tx + tw;  // inicio del panel HUD derecho

    // === Fondo oscuro en paneles HUD (diferencia del tablero negro) ===
    for (uint16_t y = 0; y < sh; y++)
        for (uint16_t x = 0; x < sw; x++)
            if (x < tx || x >= rx)
                gbt_dibujar_pixel(x, y, GA);

    // === Brillo neon en bordes del tablero ===
    for (uint16_t y = ty; y <= ty + th; y++) {
        if (tx >= 1) gbt_dibujar_pixel(tx - 1, y, CO);
        if (tx >= 2) gbt_dibujar_pixel(tx - 2, y, A);
        if (rx + 1 < sw) gbt_dibujar_pixel(rx + 1, y, CO);
        if (rx + 2 < sw) gbt_dibujar_pixel(rx + 2, y, A);
    }
    for (uint16_t x = tx; x <= rx; x++) {
        if (ty >= 1) gbt_dibujar_pixel(x, ty - 1, CO);
        if (ty >= 2) gbt_dibujar_pixel(x, ty - 2, A);
        if (ty + th + 1 < sh) gbt_dibujar_pixel(x, ty + th + 1, CO);
        if (ty + th + 2 < sh) gbt_dibujar_pixel(x, ty + th + 2, A);
    }

    // === Marcos neon parciales decorativos (esquinas tipo bracket) ===
    uint16_t clen = (sw >= 400) ? 14 : 7;
    uint16_t pad  = 3;

    // Panel izquierdo: marco superior (cyan) y marco inferior (magenta)
    neonEsquinas(pad, pad, tx - pad, sh / 3,          CC, clen, sw, sh);
    neonEsquinas(pad, sh * 2 / 3, tx - pad, sh - pad, M,  clen, sw, sh);

    // Panel derecho: marco superior (amarillo) y marco inferior (cyan)
    neonEsquinas(rx + pad, pad, sw - pad, sh / 3,          AM, clen, sw, sh);
    neonEsquinas(rx + pad, sh * 2 / 3, sw - pad, sh - pad, CC, clen, sw, sh);

    // === Acentos neon dispersos (puntos y guiones decorativos) ===
    if (tx > 4 && sh > 10) {
        uint16_t rdw = sw - rx;

        // Panel izquierdo: pequeños puntos de 2px
        if (tx / 3 < sw && sh / 5 + 1 < sh) {
            gbt_dibujar_pixel(tx / 3,      sh / 5,     A);
            gbt_dibujar_pixel(tx / 3,      sh / 5 + 1, A);
        }
        if (tx * 2 / 3 < tx && sh * 3 / 5 + 1 < sh) {
            gbt_dibujar_pixel(tx * 2 / 3,  sh * 3 / 5,     M);
            gbt_dibujar_pixel(tx * 2 / 3,  sh * 3 / 5 + 1, M);
        }
        // Guión horizontal
        for (uint16_t i = 0; i < 4; i++)
            if (tx / 5 + i < tx) gbt_dibujar_pixel(tx / 5 + i, sh * 2 / 5, CC);

        // Panel derecho: pequeños puntos de 2px
        if (rx + rdw * 3 / 4 < sw && sh / 4 + 1 < sh) {
            gbt_dibujar_pixel(rx + rdw * 3 / 4, sh / 4,     AM);
            gbt_dibujar_pixel(rx + rdw * 3 / 4, sh / 4 + 1, AM);
        }
        if (rx + rdw / 3 < sw && sh * 3 / 4 + 1 < sh) {
            gbt_dibujar_pixel(rx + rdw / 3, sh * 3 / 4,     CC);
            gbt_dibujar_pixel(rx + rdw / 3, sh * 3 / 4 + 1, CC);
        }
        // Guión horizontal
        for (uint16_t i = 0; i < 4; i++)
            if (rx + rdw * 2 / 3 + i < sw) gbt_dibujar_pixel(rx + rdw * 2 / 3 + i, sh * 3 / 5, A);
    }
}

///Funciones dibujar
void fondoDibujar(tTablero* tablero, tConfigPantalla* config)
{
    if(!tablero || !config)
    {
        return;
    }
//=======================================================
///                     Cuadriacula
//=======================================================
    uint16_t i,j,y,x;

    for(i = config->offset_tablero_y; i < config->offset_tablero_y + config->alto_tablero; i++)
    {
        for(j = config->offset_tablero_x; j < config->offset_tablero_x + config->ancho_tablero; j++)
        {
            gbt_dibujar_pixel(j, i, N);
        }
    }
    //Lineas verticales
    for(j = 0; j < config->columnas + 1; j++)
    {
        x = config->offset_tablero_x + (j * config->tam_bloque);
        for(i = 0; i < config->alto_tablero; i++)
        {
            gbt_dibujar_pixel(x,config->offset_tablero_y + i, CC);
        }
    }
    //Lineas horizontales
    for(i = 0; i < config->filas + 1; i++)
    {
        y = config->offset_tablero_y + (i * config->tam_bloque);
        for(j = 0; j < config->ancho_tablero; j++)
        {
            gbt_dibujar_pixel(config->offset_tablero_x + j, y, CC);
        }
    }
}
void bloqueDibujar(uint16_t x_pantalla, uint16_t y_pantalla, uint8_t color, tConfigPantalla* config)
{
    for(uint8_t i = 0; i < config->tam_bloque; i++)
    {
        for(uint8_t j = 0; j < config->tam_bloque; j++)
        {
            gbt_dibujar_pixel(x_pantalla + i, y_pantalla + j, color);
        }
    }
}
void piezasAncladasDibujar(tTablero* t, tConfigPantalla* config)
{
    if(!t || !(t->tablero))
    {
        return;
    }
    for(uint8_t i = 0; i < config->filas; i++)
    {
        for(uint8_t j = 0; j < config->columnas; j++)
        {
            if(t->tablero[i][j] != 0)
            {
                bloqueDibujar(config->offset_tablero_x + j * config->tam_bloque, config->offset_tablero_y + i * config->tam_bloque, t->tablero[i][j], config);
            }
        }
    }
}
void piezaActualDibujar(tTetromino* tetro, tConfigPantalla* config)
{
    if(!tetro)
    {
        return;
    }
    for(uint8_t i = 0; i < 4; i++)
    {
        for(uint8_t j = 0; j < 4; j++)
        {
            if(tetro->matriz[i][j] != 0)
            {
                bloqueDibujar(config->offset_tablero_x + (tetro->x + j) * config->tam_bloque, config->offset_tablero_y + (tetro->y + i)* config->tam_bloque, tetro->matriz[i][j], config);
            }
        }
    }
}

///Funciones logica
void tableroActualizarEstado(tTablero* t,tConfigPantalla* config, tEstadoJuego* estado, bool* game_over)
{
    if(!t || !t->pieza_actual)
    {
        return;
    }
//=======================================================
    printf("=== Antes: pieza actual: tipo=%d, x=%d, y=%d\n",
           t->pieza_actual->tipo, t->pieza_actual->x, t->pieza_actual->y);
//=======================================================
    t->pieza_actual->y++;
    if(posicionValida(t->pieza_actual, t, config))
    {
//=======================================================
        printf("    Posicion valida, continua bajando\n");
//=======================================================
        return;
    }
//=======================================================
    printf("    Posicion INVALIDA, ancla\n");
//=======================================================
    t->pieza_actual->y--;
    piezaAnclar(t->pieza_actual, t, config);
//=======================================================
    printf("    Pieza anclada en y=%d\n", t->pieza_actual->y);
//=======================================================
    if (t->pieza_actual->tipo != TETROMINO_NULO)
        estado->conteo_piezas[(int)t->pieza_actual->tipo]++;
    int limpias = lineasLimpiar(t, config, estado);
    static const int puntos_por_lineas[5] = {0, 100, 300, 500, 800};
    if (limpias >= 1 && limpias <= 4)
        estado->puntos += puntos_por_lineas[limpias];
    tetrominoCopiar(t->pieza_actual, t->pieza_siguiente);
//=======================================================
    printf("    Nueva pieza actual (era siguiente): tipo=%d, x=%d, y=%d\n",
           t->pieza_actual->tipo, t->pieza_actual->x, t->pieza_actual->y);
//=======================================================
    tetrominoAleatorio(t->pieza_siguiente, config->columnas);
//=======================================================
    printf("    Nueva pieza siguiente generada: tipo=%d, x=%d\n",
           t->pieza_siguiente->tipo, t->pieza_siguiente->x);
//=======================================================
    if(!posicionValida(t->pieza_actual, t, config))
    {
//=======================================================
        printf("    ERROR: Nueva pieza actual NO es valida! GAME OVER\n");
//=======================================================
        *game_over = true;
        return;
    }
//=======================================================
    printf("    Nueva pieza actual es valida\n\n");
//=======================================================
}
bool posicionValida(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return false;
    }
    int x_tablero, y_tablero;
    for(uint8_t i = 0; i < 4; i++)
    {
        for(uint8_t j = 0; j < 4; j++)
        {
            if(tetro->matriz[i][j])
            {
                x_tablero = tetro->x + j;
                y_tablero = tetro->y + i;
                // Validacion limites
                if(x_tablero < 0 || x_tablero >= config->columnas ||
                        y_tablero < 0 || y_tablero >= config->filas)
                {
                    return false;
                }
                // colisión
                if(t->tablero[y_tablero][x_tablero] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
///Funciones validantes
bool puedeBajar(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return false;
    }
    tetro->y++;
    bool valido = posicionValida(tetro, t, config);
    tetro->y--;
    return valido;
}
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return false;
    }
    tetro->x--;
    bool valido = posicionValida(tetro, t, config);
    tetro->x++;
    return valido;
}
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return false;
    }
    tetro->x++;
    bool valido = posicionValida(tetro, t, config);
    tetro->x--;
    return valido;
}
static bool validarMatrizEnPosicion(int x, int y, const uint8_t* matriz, tTablero* t, tConfigPantalla* config)
{

    if(!matriz || !t)
    {
        return false;
    }
    int x_tab,y_tab;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {

            if(matriz[i * 4 + j])
            {
                x_tab = x + j;
                y_tab = y + i;

                if(x_tab < 0 || x_tab >= config->columnas ||
                        y_tab < 0 || y_tab >= config->filas)
                {
                    return false;
                }
                if(t->tablero[y_tab][x_tab] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}
bool puedeRotar(tTetromino* tetro, tTablero* t, tConfigPantalla* config, bool sentido_derecha)
{
    if(!tetro || !t)
    {
        return false;
    }
    uint8_t nueva_rotacion;
    if(sentido_derecha)
    {
        nueva_rotacion = (tetro->rotacion + 1) % 4;
    }
    else
    {
        nueva_rotacion = (tetro->rotacion + 3) % 4;
    }
    const uint8_t* nueva_matriz = tetrominoObtenerMatrizRotacion(tetro->tipo, nueva_rotacion);
    if(!nueva_matriz)
    {
        return false;
    }
    return validarMatrizEnPosicion(tetro->x, tetro->y, nueva_matriz, t, config);
}

void piezaAnclar(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    uint8_t i,j,x,y;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if(tetro->matriz[i][j])
            {
                x = tetro->x + j;
                y = tetro->y + i;
                if(x >= config->columnas || y >= config->filas)
                {
                    return; //Si el bloque no cabe, cancelamos anclaje
                }
            }
        }
    }
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if(tetro->matriz[i][j])
            {
                x = tetro->x + j;
                y = tetro->y + i;
                t->tablero[y][x] = tetro->matriz[i][j];
            }
        }
    }
}
int lineasLimpiar(tTablero* t, tConfigPantalla* config, tEstadoJuego* estado)
{
    if(!t)
    {
        return 0;
    }
    int limpias = 0;
    bool llena;
    for(int y = 0; y < config->filas; y++)
    {
        llena = true;
        // verificar fila completa
        for(int x = 0; x < config->columnas; x++)
        {
            if(t->tablero[y][x] == 0)
            {
                llena = false;
                break;
            }
        }
        if(llena)
        {
            // guardar puntero de la fila eliminada
            unsigned char* filaEliminada = t->tablero[y];
            // mover punteros hacia abajo
            for(int fila = y; fila > 0; fila--)
            {
                t->tablero[fila] = t->tablero[fila - 1];
            }
            // reutilizar la fila eliminada arriba
            t->tablero[0] = filaEliminada;
            // vaciar contenido
            memset(t->tablero[0], 0, config->columnas * sizeof(unsigned char));
            estado->lineas_limpias++;
            limpias++;
            y--;
        }
    }
    return limpias;
}
