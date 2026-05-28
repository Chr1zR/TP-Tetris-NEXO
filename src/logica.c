#include "../include/errores.h"
#include "../include/logica.h"
#include "../include/juego.h"
#include "../include/config.h"
#include "../include/velocidad.h"
#include <stdlib.h>
#include <string.h>

///funciones tTablero
tTablero* crear_tablero(tConfigPantalla* config)
{
    if(!config || config->tablero.filas <= 0 || config->tablero.columnas <= 0)
    {
        return NULL;
    }
    tTablero* tablero = malloc(sizeof(tTablero));

    if(!tablero)
    {
        return NULL;
    }
    tablero->tablero = calloc(config->tablero.filas, sizeof(unsigned char *));
    if(!(tablero -> tablero))
    {
        free(tablero);
        return NULL;
    }
    for(size_t i = 0; i < config->tablero.filas; i++)
    {
        tablero->tablero[i] = calloc(config->tablero.columnas, sizeof(unsigned char));
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
        for(uint8_t i = 0; i < config->tablero.filas; i++)
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
void tableroReiniciar(tTablero* tablero, tConfigPantalla* config)
{
    if(!tablero || !tablero->tablero)
        return;

    for(uint8_t i = 0; i < config->tablero.filas; i++)
    {
        memset(tablero->tablero[i], 0, config->tablero.columnas);
    }
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

///Funciones de logica
void tableroActualizarEstado(tTablero* t, tConfigPantalla* config, tJuego* juego)
{
    if(!t || !t->pieza_actual || !juego)
    {
        return;
    }

    t->pieza_actual->y++;
    if(posicionValida(t->pieza_actual, t, config))
    {
        return;
    }
    t->pieza_actual->y--;
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
                // Validacion limites Y (siempre estricto)
                if(y_tablero < 0 || y_tablero >= config->tablero.filas)
                {
                    return false;
                }
                // Validacion limites X (circular si esta activado)
                if(config->tablero_circular)
                {
                    x_tablero = ((x_tablero % config->tablero.columnas) + config->tablero.columnas) % config->tablero.columnas;
                }
                else if(x_tablero < 0 || x_tablero >= config->tablero.columnas)
                {
                    return false;
                }
                // colision
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
    int y_original = tetro->y;
    tetro->y++;
    bool valido = posicionValida(tetro, t, config);
    tetro->y = y_original;
    return valido;
}
bool piezaTocoFondo(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return true;
    }
    tetro->y++;
    bool valido = posicionValida(tetro, t, config);
    tetro->y--;
    return !valido;
}
bool puedeMoverIzquierda(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return false;
    }
    int x_original = tetro->x;
    tetro->x--;
    bool valido = posicionValida(tetro, t, config);
    tetro->x = x_original;
    return valido;
}
bool puedeMoverDerecha(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t)
    {
        return false;
    }
    int x_original = tetro->x;
    tetro->x++;
    bool valido = posicionValida(tetro, t, config);
    tetro->x = x_original;
    return valido;
}

/**
 * @desc Valida si una matriz cabe en una posicion del tablero.
 * @param x int Coordenada X.
 * @param y int Coordenada Y.
 * @param matriz const uint8_t* Matriz 4x4 plana.
 * @param t tTablero* Puntero al tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return bool true si cabe, false si no.
 */
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

                if(y_tab < 0 || y_tab >= config->tablero.filas)
                {
                    return false;
                }
                if(config->tablero_circular)
                {
                    x_tab = ((x_tab % config->tablero.columnas) + config->tablero.columnas) % config->tablero.columnas;
                }
                else if(x_tab < 0 || x_tab >= config->tablero.columnas)
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
int puedeRotar(tTetromino* tetro, tTablero* t, tConfigPantalla* config, bool sentido_derecha)
{
    if(!tetro || !t)
    {
        return 99;
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
        return 99;
    }
    if(validarMatrizEnPosicion(tetro->x, tetro->y, nueva_matriz, t, config))
    {
        return 0;
    }
    // Wall kick: probar desplazamientos laterales si no esta activo tablero circular
    if(!config->tablero_circular)
    {
        int desplazamientos[] = {-1, 1, -2, 2};
        for(int d = 0; d < 4; d++)
        {
            if(validarMatrizEnPosicion(tetro->x + desplazamientos[d], tetro->y, nueva_matriz, t, config))
            {
                return desplazamientos[d];
            }
        }
    }
    return 99;
}

void piezaAnclar(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t || !config)
    {
        return;
    }
    const uint8_t* matriz = tetrominoObtenerMatrizRotacion(tetro->tipo,tetro->rotacion);
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(matriz[i*4+j])
            {
                int x = tetro->x+j;
                int y = tetro->y+i;
                if(x >= 0 && x < config->tablero.columnas && y >=0 && y < config->tablero.filas)
                {
                    t->tablero[y][x] = tetro->matriz[i][j];
                }
            }
        }
    }
}

uint32_t puntosConMultiplicador(uint32_t puntos_base, tJuego* juego)
{
    if(!juego || juego->intervalo_actual <= 0.0)
        return puntos_base;

    double mult = juego->config->velocidad_inicial / juego->intervalo_actual;
    return (uint32_t)(puntos_base * mult + 0.5);
}

void piezaAnclarYContinuar(tTablero* t, tConfigPantalla* config, tJuego* juego)
{
    if(!t || !t->pieza_actual || !juego)
        return;

    tEstadoJuego* estado = juego->estado;

    piezaAnclar(t->pieza_actual, t, config);
    if (t->pieza_actual->tipo != TETROMINO_NULO){
        estado->conteo_piezas[(int)t->pieza_actual->tipo]++;
        estado->piezas_totales++;
    }
    int limpias = lineasLimpiar(t, config, estado);
    static const uint32_t puntos_por_lineas[5] = {0, 100, 300, 500, 800};
    if (limpias >= 1 && limpias <= 4) {
        uint32_t base = puntos_por_lineas[limpias];
        estado->puntos += puntosConMultiplicador(base, juego);
    }

    velocidadAplicarSiCorresponde(estado, &juego->intervalo_actual, &juego->temporizador);

    tetrominoCopiar(t->pieza_actual, t->pieza_siguiente);
    tetrominoAleatorio(t->pieza_siguiente, config->tablero.columnas, config->piezas_extras);
    if(!posicionValida(t->pieza_actual, t, config))
    {
        juego->game_over = true;
        return;
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
    for(int y = 0; y < config->tablero.filas; y++)
    {
        llena = true;
        // verificar fila completa
        for(int x = 0; x < config->tablero.columnas; x++)
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
            memset(t->tablero[0], 0, config->tablero.columnas * sizeof(unsigned char));
            estado->lineas_limpias++;
            limpias++;
            y--;
        }
    }
    return limpias;
}

int piezaGhostObtenerY(tTetromino* tetro, tTablero* t, tConfigPantalla* config)
{
    if(!tetro || !t || !config)
        return tetro ? tetro->y : 0;

    tTetromino copia;
    memcpy(&copia, tetro, sizeof(tTetromino));

    while(posicionValida(&copia, t, config))
    {
        copia.y++;
    }

    return copia.y - 1;
}

int cheatLineaLimpiar(tTablero* t, tConfigPantalla* config, tEstadoJuego* estado)
{
    if(!t || !config || !estado)
        return 0;

    // Buscar la fila mas baja que tenga al menos un bloque
    int fila_objetivo = -1;
    for(int y = config->tablero.filas - 1; y >= 0; y--)
    {
        for(int x = 0; x < config->tablero.columnas; x++)
        {
            if(t->tablero[y][x] != 0)
            {
                fila_objetivo = y;
                break;
            }
        }
        if(fila_objetivo != -1)
            break;
    }

    if(fila_objetivo == -1)
        return 0; // Tablero vacio

    // Eliminar la fila (mismo algoritmo que lineasLimpiar)
    unsigned char* filaEliminada = t->tablero[fila_objetivo];
    for(int fila = fila_objetivo; fila > 0; fila--)
    {
        t->tablero[fila] = t->tablero[fila - 1];
    }
    t->tablero[0] = filaEliminada;
    memset(t->tablero[0], 0, config->tablero.columnas * sizeof(unsigned char));

    estado->lineas_limpias++;
    return 1;
}

void cheatReRoll(tTablero* t, tConfigPantalla* config)
{
    if(!t || !t->pieza_actual || !config)
        return;

    tTetromino backup;
    memcpy(&backup, t->pieza_actual, sizeof(tTetromino));

    tetrominoAleatorio(t->pieza_actual, config->tablero.columnas, config->piezas_extras);

    if(!posicionValida(t->pieza_actual, t, config))
    {
        // No cabe, restaurar original
        memcpy(t->pieza_actual, &backup, sizeof(tTetromino));
    }
}
